#pragma once
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IObjectFilter.h"

struct Detection;

class ObjectFilterByPolygon : public IObjectFilter {
 public:
  std::vector<Detection> apply(
      const std::vector<Detection>& detections,
      const std::vector<std::vector<cv::Point>>& includeZones,
      const std::vector<std::vector<cv::Point>>& excludeZones) override;

 private:
  bool isCenterInZone(const cv::Rect& box, const std::vector<cv::Point>& zone);

  bool isBoxExcluded(const cv::Rect& box,
                     const std::vector<std::vector<cv::Point>>& excludeZones);
};