#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <string_view>

#include "proto/image_analisis.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using image::ImageAnalysis;

/**
 * Загружает изображение из файла и возвращает его в виде cv::Mat,
 * а также заполняет переданные по ссылке параметры ширины, высоты и формата.
 *
 * @param filename путь к файлу изображения
 * @return        объект cv::Mat с изображением; если загрузка не удалась,
 * возвращается пустая матрица
 */
std::optional<cv::Mat> loadImageInfo(const std::string_view filename) {
  cv::Mat image = cv::imread(filename.data());
  if (image.empty()) return std::nullopt;

  if (!image.isContinuous()) {
    image = image.clone();
  }
  return image;
}

class ImageAnalysisClient {
 public:
  explicit ImageAnalysisClient(std::shared_ptr<Channel> channel)
      : stub_(ImageAnalysis::NewStub(channel)) {}

  bool UploadImage() {
    std::optional<cv::Mat> image = loadImageInfo("1.png");
    if (!image.has_value()) {
      std::cerr << "Failed to load image." << std::endl;
      return false;
    }
    image::Image request;
    request.set_pixels(image->data, image->total() * image->elemSize());
    request.set_width(image->cols);
    request.set_height(image->rows);

    image::UploadImageRequest upload_request;
    *upload_request.mutable_image() = std::move(request);
    image::UploadImageResponse response;
    ClientContext context;
    Status status = stub_->Process(&context, upload_request, &response);

    if (status.ok()) {
      const image::Image& result_image = response.result();
      cv::Mat temp(
          cv::Size(result_image.width(), result_image.height()), CV_8UC3,
          const_cast<uchar*>(
              reinterpret_cast<const uchar*>(result_image.pixels().data())));
      cv::imshow("Uploaded Image", temp);
      cv::waitKey(0);
      return true;
    } else {
      std::cerr << "Failed to upload image: " << status.error_message()
                << std::endl;
      return false;
    }
  }

 private:
  std::unique_ptr<ImageAnalysis::Stub> stub_;
};

int main(int argc, char** argv) {
  std::string target_str = "localhost:50051";
  ImageAnalysisClient client(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  if (!client.UploadImage()) {
    std::cerr << "Failed to upload image." << std::endl;
  }

  return 0;
}