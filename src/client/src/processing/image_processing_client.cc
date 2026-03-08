#include "processing/image_processing_client.h"

#include <impl/opencv_encoder.h>

#include <opencv2/opencv.hpp>

#include "impl/grpc_stream_client.h"
#include "impl/opencv_display.h"

using ImageDetection::ProcessRequest;

bool ImageProcessingClient::ProcessImage(const cv::Mat& img,
                                         const Polygons& polygons) {
  grpc::ClientContext context;

  // Устанавливаем таймаут
  context.set_deadline(std::chrono::system_clock::now() +
                       std::chrono::seconds(10));

  auto stream = grpc_client_->CreateStream(&context);

  if (!stream) {
    std::cerr << "Failed to create gRPC stream" << std::endl;
    return false;
  }

  // Отправка полигонов
  ProcessRequest request;
  auto* polygon_list = request.mutable_polygon_list();

  // Устанавливаем имя набора (имя файла)
  polygon_list->set_name(polygons.GetLastFileName());

  // Копируем полигоны из загруженных данных
  for (const auto& p : polygons.GetPolygonList().polygons()) {
    auto* poly = polygon_list->add_polygons();
    poly->CopyFrom(p);
  }

  // Копируем имена классов
  for (const auto& class_name : polygons.GetPolygonList().class_names()) {
    polygon_list->add_class_names(class_name);
  }

  if (!stream->Write(request)) {
    std::cerr << "Failed to send polygons" << std::endl;
    return false;
  }

  // Кодирование изображения
  std::optional<std::vector<uint8_t>> encoded;
  try {
    encoded = encoder_->Encode(img, ".jpg", 95);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during encoding: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Encoding error: " << e.what() << std::endl;
    return false;
  }

  if (!encoded) {
    std::cerr << "Encoding failed\n";
    return false;
  }

  const size_t chunk_size = 64 * 1024;
  size_t offset = 0;
  while (offset < encoded->size()) {
    ImageDetection::ProcessRequest chunk_req;
    size_t current = std::min(chunk_size, encoded->size() - offset);
    chunk_req.set_image_data(encoded->data() + offset, current);
    if (!stream->Write(chunk_req)) {
      std::cerr << "Failed to send chunk" << std::endl;
      return false;
    }
    offset += current;
  }
  stream->WritesDone();

  // Приём ответа
  std::vector<uint8_t> response_buffer;
  ImageDetection::ProcessResponse resp;
  while (stream->Read(&resp)) {
    const auto& chunk = resp.image_data();
    response_buffer.insert(response_buffer.end(), chunk.begin(), chunk.end());
  }

  grpc::Status status = stream->Finish();
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_message() << std::endl;
    return false;
  }

  if (response_buffer.empty()) {
    std::cerr << "No image data received" << std::endl;
    return false;
  }

  std::optional<cv::Mat> result;
  try {
    result = encoder_->Decode(response_buffer, cv::IMREAD_COLOR);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during decoding: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Decoding error: " << e.what() << std::endl;
    return false;
  }

  if (!result) {
    std::cerr << "Decoding failed\n";
    return false;
  }

  try {
    display_->Show(*result, "Processed Image");
    display_->WaitKey(0);
  } catch (const cv::Exception& e) {
    std::cerr << "OpenCV exception during display: " << e.what() << std::endl;
    return false;
  } catch (const std::exception& e) {
    std::cerr << "Display error: " << e.what() << std::endl;
    return false;
  }
  return true;
}