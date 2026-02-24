#pragma once

#include "interfaces/IDrawer.h"

class Drawer : public IDrawer {
 public:
  void draw(cv::Mat& image,
            const std::vector<ImageDetection::Polygon>& polygons) override;
};