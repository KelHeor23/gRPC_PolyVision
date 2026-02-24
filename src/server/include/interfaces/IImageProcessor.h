#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

class IImageProcessor {
 public:
  virtual ~IImageProcessor() = default;
  virtual void process(cv::Mat& image,
                       std::vector<ImageDetection::Polygon>& polygons) = 0;
};