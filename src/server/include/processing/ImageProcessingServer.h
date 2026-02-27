#pragma once
/**
 * @file ImageProcessingServer.h
 * @brief gRPC-сервер для обработки изображений.
 */
#include <grpcpp/grpcpp.h>
#include <interfaces/IImageEncoder.h>

#include <memory>

#include "interfaces/IImageProcessor.h"
#include "proto/ImageAnalysis.grpc.pb.h"

/**
 * @class ImageProcessingServer
 * @brief Реализация gRPC-сервиса ImageProcessing.
 *
 * Обрабатывает двунаправленный поток: получает полигоны, затем изображение
 * чанками, выполняет обработку и возвращает результат также чанками.
 */
class ImageProcessingServer final
    : public ImageDetection::ImageProcessing::Service {
 public:
  /**
   * @brief Конструктор с зависимостями.
   * @param encoder Кодировщик/декодировщик изображений.
   * @param processor Обработчик изображения (детекция + фильтрация).
   */
  ImageProcessingServer(std::unique_ptr<IImageEncoder> encoder,
                        std::unique_ptr<IImageProcessor> processor)
      : encoder_(std::move(encoder)), processor_(std::move(processor)) {}

  /**
   * @brief Обработка потока ProcessImage.
   * @param context Контекст gRPC.
   * @param stream Поток чтения/записи.
   * @return Статус завершения.
   */
  grpc::Status ProcessImage(
      grpc::ServerContext* context,
      grpc::ServerReaderWriter<ImageDetection::ProcessResponse,
                               ImageDetection::ProcessRequest>* stream)
      override;

 private:
  std::unique_ptr<IImageEncoder> encoder_;  ///< Кодировщик/декодировщик
  std::unique_ptr<IImageProcessor> processor_;  ///< Обработчик изображения
};