#pragma once

/**
 * @file GrpcStreamClient.h
 * @brief Реализация клиента gRPC-потока.
 */

#include <grpcpp/grpcpp.h>

#include <memory>

#include "interfaces/IGrpcStreamClient.h"

/**
 * @class GrpcStreamClient
 * @brief Конкретная реализация интерфейса IGrpcStreamClient.
 *
 * Создаёт gRPC-канал и предоставляет метод для установки двунаправленного
 * потока с сервером обработки изображений.
 */

class GrpcStreamClient : public IGrpcStreamClient {
 public:
  /**
   * @brief Конструктор, инициализирующий stub.
   * @param channel gRPC-канал для связи с сервером.
   */
  explicit GrpcStreamClient(std::shared_ptr<grpc::Channel> channel);

  /**
   * @brief Создаёт поток для обмена сообщениями с сервером.
   * @param context Контекст вызова gRPC
   * @return Умный указатель на объект потока client-reader-writer.
   */
  std::unique_ptr<grpc::ClientReaderWriter<ImageDetection::ProcessRequest,
                                           ImageDetection::ProcessResponse>>
  CreateStream(grpc::ClientContext* context) override;

 private:
  std::unique_ptr<ImageDetection::ImageProcessing::Stub>
      stub_;  ///< gRPC-заглушка для сервиса ImageProcessing.
};