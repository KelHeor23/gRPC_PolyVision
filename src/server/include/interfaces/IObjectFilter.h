#pragma once

#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;

class IObjectFilter {
 public:
  virtual ~IObjectFilter() = default;
  virtual std::vector<Detection> apply(
      const std::vector<Detection>& detections,
      const std::vector<ImageDetection::Polygon>& polygons,
      cv::Size imageSize) = 0;
};