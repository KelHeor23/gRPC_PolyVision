#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IDrawer.h"
#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
class IClassMapper;

class Drawer : public IDrawer {
 public:
  void drawDetections(cv::Mat& image, const std::vector<Detection>& detections,
                      std::shared_ptr<IClassMapper> mapper) override;

  void drawPolygons(
      cv::Mat& image,
      const std::vector<ImageDetection::Polygon>& polygons) override;
};