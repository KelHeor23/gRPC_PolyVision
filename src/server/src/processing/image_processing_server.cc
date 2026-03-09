#include "processing/image_processing_server.h"

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

    const auto& polygon_list = request.polygon_list();
    // Извлекаем полигоны
    std::vector<ImageDetection::Polygon> polygons(
        polygon_list.polygons().begin(), polygon_list.polygons().end());

    // Сбор всех чанков изображения
    std::vector<uint8_t> image_buffer;
    while (stream->Read(&request)) {
      if (request.has_image_data()) {
        const auto& chunk = request.image_data();
        image_buffer.insert(image_buffer.end(), chunk.begin(), chunk.end());
      } else {
        return Status(grpc::StatusCode::INVALID_ARGUMENT,
                      "Expected image data");
      }
    }

    // Декодирование
    auto img_opt = encoder_->Decode(image_buffer, cv::IMREAD_COLOR);
    if (!img_opt) {
      return Status(grpc::StatusCode::INVALID_ARGUMENT,
                    "Failed to decode image");
    }
    cv::Mat img = std::move(*img_opt);

    // Обработка изображения с передачей полигонов, имени и имён классов
    processor_->Process(img, polygons);

    // Кодирование результата
    auto encoded_opt = encoder_->Encode(img, ".jpg", 95);

    if (!encoded_opt) {
      return Status(grpc::StatusCode::INTERNAL, "Failed to encode result");
    }

    const auto& output_buffer = *encoded_opt;

    // Отправка чанками
    const size_t chunk_size = 64 * 1024;
    size_t offset = 0;

    while (offset < output_buffer.size()) {
      ProcessResponse response;
      size_t current = std::min(chunk_size, output_buffer.size() - offset);

      response.set_image_data(output_buffer.data() + offset, current);
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