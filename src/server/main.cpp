/**
 * @file main.cpp
 * @brief Точка входа серверного приложения.
 *
 * Инициализирует все компоненты: маппер классов, детектор YOLO, фильтр,
 * отрисовщик и запускает gRPC-сервер.
 */
#include <grpcpp/grpcpp.h>
#include <impl/OpenCVEncoder.h>

#include "impl/ClassMapper.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/ObjectsDetecting.h"
#include "impl/PolygonProcessor.h"
#include "impl/YoloDetector.h"
#include "include/processing/ImageProcessingServer.h"

/**
 * @brief Запускает gRPC-сервер.
 */
void RunServer() {
  std::string server_address("0.0.0.0:50051");

  Config conf;
  conf.modelWeights = "../../../YOLO/yolov4.weights";
  conf.modelConfig = "../../../YOLO/yolov4.cfg";
  conf.classesFile = "../../../YOLO/coco.names";

  auto classMapper = std::make_shared<ClassMapper>(conf.classesFile);
  auto yoloDetector = std::make_unique<YoloDetector>(conf, classMapper);
  auto polygonProcessor = std::make_unique<PolygonProcessor>();
  auto objectFilter =
      std::make_unique<ObjectFilterByPolygon>(std::move(polygonProcessor));
  auto drawer = std::make_unique<Drawer>();

  auto objectDetector = std::make_unique<ObjectsDetecting>(
      conf, classMapper, std::move(yoloDetector), std::move(objectFilter),
      std::move(drawer));

  // Включаем отрисовку полигонов для отладки
  objectDetector->setDrawPolygons(true);

  ImageProcessingServer service(std::make_unique<OpenCVEncoder>(),
                                std::move(objectDetector));

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

/**
 * @brief Главная функция.
 */
int main(int argc, char** argv) {
  RunServer();
  return 0;
}