#pragma once
#include <memory>
#include <opencv2/core.hpp>
#include <vector>

#include "polygons/Polygons.h"

class IGrpcStreamClient;
class IImageEncoder;
class IDisplay;

class ImageProcessingClient {
 public:
  ImageProcessingClient(std::unique_ptr<IGrpcStreamClient> grpcClient,
                        std::unique_ptr<IImageEncoder> encoder,
                        std::unique_ptr<IDisplay> display)
      : grpcClient_(std::move(grpcClient)),
        encoder_(std::move(encoder)),
        display_(std::move(display)) {}

  bool ProcessImage(const cv::Mat& img, const Polygons& polygons);

 private:
  std::unique_ptr<IGrpcStreamClient> grpcClient_;
  std::unique_ptr<IImageEncoder> encoder_;
  std::unique_ptr<IDisplay> display_;
};