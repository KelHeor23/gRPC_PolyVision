#pragma once

#include "interfaces/IImageProcessor.h"

class ObjectsDetecting : public IImageProcessor {
 public:
  void process(cv::Mat& image,
               std::vector<ImageDetection::Polygon>& polygons) override;
};