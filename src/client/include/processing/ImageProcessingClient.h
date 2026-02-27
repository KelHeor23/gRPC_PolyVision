#pragma once
/**
 * @file ImageProcessingClient.h
 * @brief Клиент для обработки изображений через gRPC.
 */
#include <memory>
#include <opencv2/core.hpp>
#include <vector>

#include "polygons/Polygons.h"

class IGrpcStreamClient;
class IImageEncoder;
class IDisplay;

/**
 * @class ImageProcessingClient
 * @brief Координирует отправку изображения и полигонов на сервер,
 *        получение результата и его отображение.
 */
class ImageProcessingClient {
 public:
  /**
   * @brief Конструктор с внедрением зависимостей.
   * @param grpcClient Клиент gRPC-потока.
   * @param encoder Кодировщик/декодировщик изображений.
   * @param display Отображение результата.
   */
  ImageProcessingClient(std::unique_ptr<IGrpcStreamClient> grpcClient,
                        std::unique_ptr<IImageEncoder> encoder,
                        std::unique_ptr<IDisplay> display)
      : grpcClient_(std::move(grpcClient)),
        encoder_(std::move(encoder)),
        display_(std::move(display)) {}

  /**
   * @brief Отправляет изображение и полигоны на сервер, получает обработанное
   * изображение.
   * @param img Исходное изображение (cv::Mat).
   * @param polygons Набор полигонов.
   * @return true при успешной обработке, false при ошибке.
   */
  bool ProcessImage(const cv::Mat& img, const Polygons& polygons);

 private:
  std::unique_ptr<IGrpcStreamClient>
      grpcClient_;  ///< Клиент для создания gRPC-потока
  std::unique_ptr<IImageEncoder>
      encoder_;  ///< Кодирование/декодирование изображений
  std::unique_ptr<IDisplay> display_;  ///< Отображение результата
};