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
#include "impl/CmdOptions.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/ObjectsDetecting.h"
#include "impl/PolygonProcessor.h"
#include "impl/YoloDetector.h"
#include "include/processing/ImageProcessingServer.h"

/**
 * @brief Запускает gRPC-сервер.
 */
void RunServer(int argc, char** argv) {
  try {
    CommandOptions options;

    options.Parse(argc, argv);

    if (options.isHelpRequested()) {
      options.printHelp();
      return;
    }

    std::string server_address("0.0.0.0:50051");

    Config conf;
    conf.modelWeights = "YOLO/yolov4.weights";
    conf.modelConfig = "YOLO/yolov4.cfg";
    conf.classesFile = "YOLO/coco.names";

    auto classMapper = std::make_shared<ClassMapper>(conf.classesFile);
    auto yoloDetector = std::make_unique<YoloDetector>(conf, classMapper);
    auto polygonProcessor = std::make_unique<PolygonProcessor>();
    auto objectFilter =
        std::make_unique<ObjectFilterByPolygon>(std::move(polygonProcessor));
    auto drawer = std::make_unique<Drawer>();

    auto objectDetector = std::make_unique<ObjectsDetecting>(
        classMapper, std::move(yoloDetector), std::move(objectFilter),
        std::move(drawer));

    bool isShowPolygons = options.GetShowPolygons();
    // Включаем отрисовку полигонов для отладки
    objectDetector->setDrawPolygons(isShowPolygons);

    ImageProcessingServer service(std::make_unique<OpenCVEncoder>(),
                                  std::move(objectDetector));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
  } catch (const std::exception& e) {
    throw;
  }
}

/**
 * @brief Главная функция.
 */
int main(int argc, char** argv) {
  try {
    RunServer(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}