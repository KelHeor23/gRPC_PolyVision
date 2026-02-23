#include "processing/ImageProcessingClient.h"

#include <opencv2/opencv.hpp>

using ImageDetection::ProcessRequest;

bool ImageProcessingClient::ProcessImage(const cv::Mat& img,
                                         const Polygons& polygons) {
  grpc::ClientContext context;
  auto stream = grpcClient_->CreateStream(&context);

  // Отправка полигонов
  ProcessRequest request;
  auto* polygon_list = request.mutable_polygon_list();
  for (const auto& p : polygons.getPolygons()) {
    auto* poly = polygon_list->add_polygons();
    poly->CopyFrom(p);
  }

  if (!stream->Write(request)) {
    std::cerr << "Failed to send polygons" << std::endl;
    return false;
  }

  // Кодирование изображения
  std::vector<uint8_t> encoded = encoder_->encode(img, ".jpg", 95);
  const size_t chunkSize = 64 * 1024;
  size_t offset = 0;
  while (offset < encoded.size()) {
    ImageDetection::ProcessRequest chunkReq;
    size_t current = std::min(chunkSize, encoded.size() - offset);
    chunkReq.set_image_data(encoded.data() + offset, current);
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

  cv::Mat result = encoder_->decode(responseBuffer, cv::IMREAD_COLOR);
  if (result.empty()) {
    std::cerr << "Failed to decode result" << std::endl;
    return false;
  }

  display_->show(result, "Processed Image");
  display_->waitKey(0);
  return true;
}