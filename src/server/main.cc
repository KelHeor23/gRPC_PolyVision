/**
 * @file main.cpp
 * @brief Точка входа серверного приложения.
 *
 * Инициализирует все компоненты: маппер классов, детектор YOLO, фильтр,
 * отрисовщик и запускает gRPC-сервер.
 */
#include <grpcpp/grpcpp.h>
#include <impl/opencv_encoder.h>

#include "impl/class_mapper.h"
#include "impl/cmd_options.h"
#include "impl/drawer.h"
#include "impl/object_filter_by_polygon.h"
#include "impl/objects_detecting.h"
#include "impl/polygon_processor.h"
#include "impl/yolo_detector.h"
#include "include/processing/image_processing_server.h"

/**
 * @brief Запускает gRPC-сервер.
 */
void RunServer(int argc, char** argv) {
  try {
    CommandOptions options;

    options.Parse(argc, argv);

    if (options.IsHelpRequested()) {
      options.PrintHelp();
      return;
    }

    std::string server_address("0.0.0.0:50051");

    Config conf;
    conf.model_weights = "YOLO/yolov4.weights";
    conf.model_config = "YOLO/yolov4.cfg";
    conf.classes_file = "YOLO/coco.names";

    auto class_mapper = std::make_shared<ClassMapper>(conf.classes_file);
    auto yolo_detector = std::make_unique<YoloDetector>(conf, class_mapper);
    auto polygon_processor = std::make_unique<PolygonProcessor>();
    auto object_filter =
        std::make_unique<ObjectFilterByPolygon>(std::move(polygon_processor));
    auto drawer = std::make_unique<Drawer>();

    auto object_detector = std::make_unique<ObjectsDetecting>(
        class_mapper, std::move(yolo_detector), std::move(object_filter),
        std::move(drawer));

    bool is_show_polygons = options.GetShowPolygons();
    // Включаем отрисовку полигонов для отладки
    object_detector->SetDrawPolygons(is_show_polygons);

    ImageProcessingServer service(std::make_unique<OpenCVEncoder>(),
                                  std::move(object_detector));

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