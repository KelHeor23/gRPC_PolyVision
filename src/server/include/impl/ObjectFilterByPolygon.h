#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IObjectFilter.h"
#include "interfaces/IPolygonProcessor.h"

struct Detection;

struct BinaryPolygon {
  bool isInclusion;  // true для зон включения, false для исключения
  float threshold;  // доля площади для принадлежности (0..1)
  cv::Mat mask;  // бинарная маска полигона
  cv::Rect boundingBox;  // охватывающий прямоугольник полигона
};

class ObjectFilterByPolygon : public IObjectFilter {
 public:
  ObjectFilterByPolygon(std::unique_ptr<IPolygonProcessor> polygonProcessor)
      : polygonProcessor_(std::move(polygonProcessor)) {}

  std::vector<Detection> apply(const std::vector<Detection>& detections,
                               std::vector<ImageDetection::Polygon>& polygons,
                               cv::Size imageSize,
                               const std::string& polygonsName) override;

 private:
  BinaryPolygon createBinaryPolygon(const ImageDetection::Polygon& poly,
                                    cv::Size imageSize);

  bool isDetectionAccepted(const Detection& detection,
                           const std::vector<BinaryPolygon>& polygons);

  std::unique_ptr<IPolygonProcessor> polygonProcessor_;
  std::vector<BinaryPolygon> binaryPolygons_;
  std::string lastPolygonsName_;
};