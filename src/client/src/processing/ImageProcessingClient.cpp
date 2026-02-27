#include "processing/ImageProcessingClient.h"

#include <impl/OpenCVEncoder.h>

#include <opencv2/opencv.hpp>

#include "impl/GrpcStreamClient.h"
#include "impl/OpenCVDisplay.h"

using ImageDetection::ProcessRequest;

bool ImageProcessingClient::ProcessImage(const cv::Mat& img,
                                         const Polygons& polygons) {
  grpc::ClientContext context;

  // Устанавливаем таймаут
  context.set_deadline(std::chrono::system_clock::now() +
                       std::chrono::seconds(10));

  auto stream = grpcClient_->CreateStream(&context);

  if (!stream) {
    std::cerr << "Failed to create gRPC stream" << std::endl;
    return false;
  }

  // Отправка полигонов
  ProcessRequest request;
  auto* polygon_list = request.mutable_polygon_list();

  // Записываю имя набора полигонов
  polygon_list->set_name(polygons.getLastFileName());
  for (const auto& p : polygons.getPolygons()) {
    auto* poly = polygon_list->add_polygons();
    poly->CopyFrom(p);
  }

  if (!stream->Write(request)) {
    std::cerr << "Failed to send polygons" << std::endl;
    return false;
  }

  // Кодирование изображения
  std::optional<std::vector<uint8_t>> encoded;
  try {
    encoded = encoder_->encode(img, ".jpg", 95);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during encoding: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Encoding error: " << e.what() << std::endl;
    return false;
  }

  const size_t chunkSize = 64 * 1024;
  size_t offset = 0;
  while (offset < encoded->size()) {
    ImageDetection::ProcessRequest chunkReq;
    size_t current = std::min(chunkSize, encoded->size() - offset);
    chunkReq.set_image_data(encoded->data() + offset, current);
    if (!stream->Write(chunkReq)) {
      std::cerr << "Failed to send chunk" << std::endl;
      return false;
    }
    offset += current;
  }
  stream->WritesDone();

  // Приём ответа
  std::vector<uint8_t> responseBuffer;
  ImageDetection::ProcessResponse resp;
  while (stream->Read(&resp)) {
    const auto& chunk = resp.image_data();
    responseBuffer.insert(responseBuffer.end(), chunk.begin(), chunk.end());
  }

  grpc::Status status = stream->Finish();
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_message() << std::endl;
    return false;
  }

  if (responseBuffer.empty()) {
    std::cerr << "No image data received" << std::endl;
    return false;
  }

  std::optional<cv::Mat> result;
  try {
    result = encoder_->decode(responseBuffer, cv::IMREAD_COLOR);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during decoding: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Decoding error: " << e.what() << std::endl;
    return false;
  }

  try {
    display_->show(*result, "Processed Image");
    display_->waitKey(0);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during display: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Display error: " << e.what() << std::endl;
    return false;
  }
  return true;
}