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
#include "impl/detection_output_parser.h"
#include "impl/drawer.h"
#include "impl/geometric_filter_by_polygon.h"
#include "impl/image_preprocessor.h"
#include "impl/inference_backend.h"
#include "impl/objects_detecting.h"
#include "impl/polygon_clipper_sutherland_hodgman.h"
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

    Config conf{.model_weights = "YOLO/yolov4.weights",
                .model_config = "YOLO/yolov4.cfg",
                .classes_file = "YOLO/coco.names"};

    auto preprocessor = std::make_unique<YoloPreprocessor>(conf.input_size);
    auto backend = std::make_unique<OpenCvDnnBackend>();
    backend->loadModel(conf.model_config, conf.model_weights);
    auto parser = std::make_unique<YoloOutputParser>();

    auto yolo_detector = std::make_unique<YoloDetector>(
        std::move(preprocessor), std::move(backend), std::move(parser),
        conf.conf_threshold, conf.nms_threshold);

    auto class_mapper = std::make_shared<ClassMapper>(conf.classes_file);
    auto object_filter = std::make_unique<GeometricFilterByPolygon>(
        std::make_unique<SutherlandHodgmanClipper>());

    auto object_detector = std::make_unique<ObjectsDetecting>(
        class_mapper, std::move(yolo_detector), std::move(object_filter),
        std::make_unique<Drawer>(),
        std::make_unique<PolygonProcessor>(class_mapper));

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