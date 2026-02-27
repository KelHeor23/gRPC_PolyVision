#pragma once
/**
 * @file IGrpcStreamClient.h
 * @brief Интерфейс клиента gRPC-потока.
 */
#include <grpcpp/grpcpp.h>

#include "proto/ImageAnalysis.grpc.pb.h"

/**
 * @interface IGrpcStreamClient
 * @brief Определяет метод для создания двунаправленного потока с сервером.
 */
class IGrpcStreamClient {
 public:
  virtual ~IGrpcStreamClient() = default;
  /**
   * @brief Создаёт поток для обмена ProcessRequest / ProcessResponse.
   * @param context Контекст gRPC.
   * @return Поток для чтения и записи сообщений.
   */
  virtual std::unique_ptr<grpc::ClientReaderWriter<
      ImageDetection::ProcessRequest, ImageDetection::ProcessResponse>>
  CreateStream(grpc::ClientContext* context) = 0;
};