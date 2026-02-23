#include <memory>
#include <opencv2/imgcodecs.hpp>

#include "include/impl/GrpcStreamClient.h"
#include "include/impl/OpencvCodec.h"
#include "include/impl/OpencvDisplay.h"
#include "include/processing/ImageProcessingClient.h"

int main() {
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  auto grpcClient = std::make_unique<GrpcStreamClient>(channel);
  auto encoder = std::make_unique<OpenCVEncoder>();
  auto display = std::make_unique<OpenCVDisplay>();

  ImageProcessingClient client(std::move(grpcClient), std::move(encoder),
                               std::move(display));

  cv::Mat image = cv::imread("../../../images/1.png");
  Polygons polygons;
  if (!polygons.loadFromFile("../../../images/1.json")) {
    std::cerr << "Failed to load polygons from file" << std::endl;
    return 1;
  }
  client.ProcessImage(image, polygons);
  return 0;
}