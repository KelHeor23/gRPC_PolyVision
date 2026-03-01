#include "processing/ImageProcessingServer.h"

#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace ImageDetection;
using grpc::Status;

Status ImageProcessingServer::ProcessImage(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<ProcessResponse, ProcessRequest>* stream) {
  try {
    // Чтение первого сообщения с полигонами
    ProcessRequest request;
    if (!stream->Read(&request)) {
      return Status(grpc::StatusCode::INVALID_ARGUMENT,
                    "Expected polygons first");
    }
    if (!request.has_polygon_list()) {
      return Status(grpc::StatusCode::INVALID_ARGUMENT,
                    "First message must contain polygons");
    }

    const auto& src = request.polygon_list().polygons();
    std::vector<ImageDetection::Polygon> polygons(src.begin(), src.end());

    std::string polygonsName = request.polygon_list().name();

    // Сбор всех чанков изображения
    std::vector<uint8_t> imageBuffer;
    while (stream->Read(&request)) {
      if (request.has_image_data()) {
        const auto& chunk = request.image_data();
        imageBuffer.insert(imageBuffer.end(), chunk.begin(), chunk.end());
      } else {
        return Status(grpc::StatusCode::INVALID_ARGUMENT,
                      "Expected image data");
      }
    }

    // Декодирование
    auto imgOpt = encoder_->decode(imageBuffer, cv::IMREAD_COLOR);
    if (!imgOpt) {
      return Status(grpc::StatusCode::INVALID_ARGUMENT,
                    "Failed to decode image");
    }
    cv::Mat img = std::move(*imgOpt);

    // Обработка изображения
    processor_->process(img, polygons, polygonsName);

    // Кодирование результата
    auto encodedOpt = encoder_->encode(img, ".jpg", 95);

    if (!encodedOpt) {
      return Status(grpc::StatusCode::INTERNAL, "Failed to encode result");
    }

    const auto& outputBuffer = *encodedOpt;

    // Отправка чанками
    const size_t chunkSize = 64 * 1024;
    size_t offset = 0;

    while (offset < outputBuffer.size()) {
      ProcessResponse response;
      size_t current = std::min(chunkSize, outputBuffer.size() - offset);

      response.set_image_data(outputBuffer.data() + offset, current);
      if (!stream->Write(response)) {
        std::cerr << "Failed to send chunk" << std::endl;
        return grpc::Status(grpc::StatusCode::INTERNAL,
                            "Failed to send response chunk");
      }
      offset += current;
    }

    return Status::OK;
  } catch (const cv::Exception& e) {
    return grpc::Status(grpc::StatusCode::INTERNAL,
                        "OpenCV error: " + std::string(e.what()));
  } catch (const std::exception& e) {
    return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
  } catch (...) {
    return grpc::Status(grpc::StatusCode::UNKNOWN, "Unknown error");
  }
}