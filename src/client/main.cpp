/**
 * @file main.cpp
 * @brief Точка входа клиентского приложения.
 *
 * Программа загружает изображение и JSON с полигонами, создаёт gRPC-клиент
 * и запускает обработку на сервере, после чего отображает результат.
 */
#include <impl/OpenCVEncoder.h>

#include <memory>
#include <opencv2/imgcodecs.hpp>

#include "impl/CmdOptions.h"
#include "include/impl/GrpcStreamClient.h"
#include "include/impl/OpenCVDisplay.h"
#include "include/processing/ImageProcessingClient.h"

/**
 * @brief Главная функция.
 */
int main(int argc, char *argv[]) {
  try {
    CommandOptions options;

    options.Parse(argc, argv);

    if (options.isHelpRequested()) {
      options.printHelp();
      return 0;
    }

    auto channel = grpc::CreateChannel("server:50051",
                                       grpc::InsecureChannelCredentials());
    auto grpcClient = std::make_unique<GrpcStreamClient>(channel);
    auto encoder = std::make_unique<OpenCVEncoder>();
    auto display = std::make_unique<OpenCVDisplay>();

    ImageProcessingClient client(std::move(grpcClient), std::move(encoder),
                                 std::move(display));

    std::string imgFilePath = options.GetImageFile();
    std::string polygonsFilePath = options.GetPolygonsFile();

    if (imgFilePath.empty()) imgFilePath = "images/2.jpg";
    if (polygonsFilePath.empty()) polygonsFilePath = "images/2.json";

    cv::Mat image = cv::imread(imgFilePath);
    Polygons polygons;
    if (!polygons.loadFromFile(polygonsFilePath)) {
      std::cerr << "Failed to load polygons from file" << std::endl;
      return 1;
    }
    client.ProcessImage(image, polygons);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}