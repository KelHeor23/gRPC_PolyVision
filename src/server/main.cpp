#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "proto/image_analisis.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using image::Image;
using image::ImageAnalysis;
using image::UploadImageRequest;
using image::UploadImageResponse;

class CallData {
 public:
  // Конструктор инициализирует запрос и регистрирует его в CompletionQueue
  CallData(ImageAnalysis::AsyncService* service, ServerCompletionQueue* cq)
      : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
    // Начинаем асинхронный приём запроса
    service_->RequestProcess(&ctx_, &request_, &responder_, cq_, cq_, this);
  }

  // Метод, вызываемый при получении события из CompletionQueue
  void Proceed() {
    if (status_ == CREATE) {
      // Новый запрос только что зарегистрирован.
      // Переводим состояние в PROCESS и создаём новый CallData для
      // следующего запроса, чтобы параллельно обрабатывать несколько.
      status_ = PROCESS;

      // Создаём новый объект для следующего запроса (он сам зарегистрируется)
      new CallData(service_, cq_);

      // Выполняем обработку текущего запроса.
      // В данном случае просто копируем присланное изображение в ответ.
      Image* result = response_.mutable_result();
      result->set_pixels(request_.image().pixels());
      result->set_width(request_.image().width());
      result->set_height(request_.image().height());

      // Инициируем отправку ответа. После завершения будет событие FINISH.
      responder_.Finish(response_, Status::OK, this);

    } else if (status_ == PROCESS) {
      // Завершение отправки ответа.
      status_ = FINISH;
      // Здесь можно было бы выполнить очистку, но объект будет удалён ниже.

    } else {
      // Состояние FINISH – запрос полностью обработан.
      // Удаляем объект, так как он больше не нужен.
      delete this;
    }
  }

 private:
  ImageAnalysis::AsyncService* service_;
  ServerCompletionQueue* cq_;
  ServerContext ctx_;

  // Запрос и ответ
  UploadImageRequest request_;
  UploadImageResponse response_;

  // Асинхронный отправитель ответа для унарного RPC
  grpc::ServerAsyncResponseWriter<UploadImageResponse> responder_;

  // Состояние обработки запроса
  enum CallStatus { CREATE, PROCESS, FINISH };
  CallStatus status_;
};

// Класс сервера
class AsyncImageServer {
 public:
  AsyncImageServer(const std::string& server_address, int num_threads)
      : server_address_(server_address), num_threads_(num_threads) {}

  ~AsyncImageServer() {
    server_->Shutdown();
    cq_->Shutdown();
    for (auto& thread : threads_) {
      thread.join();
    }
  }

  void Run() {
    ServerBuilder builder;
    builder.AddListeningPort(server_address_,
                             grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    // Получаем CompletionQueue
    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();
    std::cout << "Async server listening on " << server_address_ << std::endl;

    // Создаём первый объект CallData для начала обработки запросов
    new CallData(&service_, cq_.get());

    // Запускаем пул потоков для обработки событий из CompletionQueue
    for (int i = 0; i < num_threads_; ++i) {
      threads_.emplace_back(&AsyncImageServer::HandleRpcs, this);
    }

    // Ожидаем завершения (можно добавить обработчик сигналов)
    for (auto& thread : threads_) {
      thread.join();
    }
  }

 private:
  void HandleRpcs() {
    void* tag;
    bool ok;
    while (true) {
      // Блокируемся до получения следующего события
      if (!cq_->Next(&tag, &ok)) {
        // CompletionQueue завершена
        break;
      }
      // В gRPC при завершении сервера ok может быть false для некоторых тегов,
      // но мы всё равно должны вызвать Proceed для корректной очистки.
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::string server_address_;
  int num_threads_;
  std::unique_ptr<Server> server_;
  std::unique_ptr<ServerCompletionQueue> cq_;
  ImageAnalysis::AsyncService service_;
  std::vector<std::thread> threads_;
};

int main(int argc, char** argv) {
  std::string server_address = "0.0.0.0:50051";
  int num_threads = 4;  // Количество потоков обработчиков

  if (argc > 1) {
    server_address = argv[1];
  }
  if (argc > 2) {
    num_threads = std::stoi(argv[2]);
  }

  AsyncImageServer server(server_address, num_threads);
  server.Run();

  return 0;
}