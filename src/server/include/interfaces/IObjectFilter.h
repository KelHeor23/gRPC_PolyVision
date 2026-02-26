#pragma once

#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;

class IObjectFilter {
 public:
  virtual ~IObjectFilter() = default;
  virtual std::vector<Detection> apply(
      const std::vector<Detection>& detections,
      std::vector<ImageDetection::Polygon>& polygons, cv::Size imageSize,
      const std::string& polygonsName) = 0;
};