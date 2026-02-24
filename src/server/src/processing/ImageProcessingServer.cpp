#include "processing/ImageProcessingServer.h"

#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace ImageDetection;
using grpc::Status;

Status ImageProcessingServer::ProcessImage(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<ProcessResponse, ProcessRequest>* stream) {
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

  std::cout << "Received " << polygons.size() << " polygons" << std::endl;

  // Сбор всех чанков изображения
  std::vector<uint8_t> imageBuffer;
  while (stream->Read(&request)) {
    if (request.has_image_data()) {
      const auto& chunk = request.image_data();
      imageBuffer.insert(imageBuffer.end(), chunk.begin(), chunk.end());
    } else {
      return Status(grpc::StatusCode::INVALID_ARGUMENT, "Expected image data");
    }
  }

  std::cout << "Total image size: " << imageBuffer.size() << " bytes"
            << std::endl;

  // Декодирование
  auto imgOpt = encoder_->decode(imageBuffer, cv::IMREAD_COLOR);
  if (!imgOpt) {
    return Status(grpc::StatusCode::INVALID_ARGUMENT, "Failed to decode image");
  }
  cv::Mat img = std::move(*imgOpt);
  std::cout << "Image decoded: " << img.cols << "x" << img.rows << std::endl;

  // Обработка изображения
  processor_->process(img, polygons);

  // Кодирование результата
  auto encodedOpt = encoder_->encode(img, ".jpg", 95);

  if (!encodedOpt) {
    return Status(grpc::StatusCode::INTERNAL, "Failed to encode result");
  }

  const auto& outputBuffer = *encodedOpt;
  std::cout << "Encoded image size: " << outputBuffer.size() << " bytes"
            << std::endl;

  // Отправка чанками
  const size_t chunkSize = 64 * 1024;
  size_t offset = 0;

  while (offset < outputBuffer.size()) {
    ProcessResponse response;
    size_t current = std::min(chunkSize, outputBuffer.size() - offset);
    response.set_image_data(outputBuffer.data() + offset, current);
    if (!stream->Write(response)) {
      std::cerr << "Failed to send chunk" << std::endl;
      break;
    }
    offset += current;
  }

  std::cout << "Processing finished, sent " << outputBuffer.size()
            << " bytes back" << std::endl;
  return Status::OK;
}