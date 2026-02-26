#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IObjectFilter.h"

struct Detection;
namespace {
struct BinaryPolygon {
  bool isInclusion;  // true для зон включения, false для исключения
  float threshold;  // доля площади для принадлежности (0..1)
  cv::Mat mask;  // бинарная маска полигона
  cv::Rect boundingBox;  // охватывающий прямоугольник полигона
};
};  // namespace

class ObjectFilterByPolygon : public IObjectFilter {
 public:
  std::vector<Detection> apply(
      const std::vector<Detection>& detections,
      const std::vector<ImageDetection::Polygon>& polygons,
      cv::Size imageSize) override;

 private:
  BinaryPolygon createBinaryPolygon(const ImageDetection::Polygon& poly,
                                    cv::Size imageSize);

  bool isDetectionAccepted(const Detection& detection,
                           const std::vector<BinaryPolygon>& polygons);
};