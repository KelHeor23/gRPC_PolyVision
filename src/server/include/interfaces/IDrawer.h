#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

class IDrawer {
 public:
  virtual ~IDrawer() = default;
  virtual void draw(cv::Mat& image,
                    const std::vector<ImageDetection::Polygon>& polygons) = 0;
};