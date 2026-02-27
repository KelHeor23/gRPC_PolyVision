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

#include "include/impl/GrpcStreamClient.h"
#include "include/impl/OpencvDisplay.h"
#include "include/processing/ImageProcessingClient.h"

/**
 * @brief Главная функция.
 */

int main() {
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  auto grpcClient = std::make_unique<GrpcStreamClient>(channel);
  auto encoder = std::make_unique<OpenCVEncoder>();
  auto display = std::make_unique<OpenCVDisplay>();

  ImageProcessingClient client(std::move(grpcClient), std::move(encoder),
                               std::move(display));

  cv::Mat image = cv::imread("../../../images/2.jpg");
  Polygons polygons;
  if (!polygons.loadFromFile("../../../images/2.json")) {
    std::cerr << "Failed to load polygons from file" << std::endl;
    return 1;
  }
  client.ProcessImage(image, polygons);
  return 0;
}