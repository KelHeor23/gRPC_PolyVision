#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
class IClassMapper;

class IDrawer {
 public:
  virtual ~IDrawer() = default;
  virtual void drawDetections(cv::Mat& image,
                              const std::vector<Detection>& detections,
                              std::shared_ptr<IClassMapper> mapper) = 0;

  virtual void drawPolygons(
      cv::Mat& image, const std::vector<ImageDetection::Polygon>& polygons) = 0;
};