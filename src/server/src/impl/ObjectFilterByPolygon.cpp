#include "impl/ObjectFilterByPolygon.h"

#include <opencv2/imgproc.hpp>

#include "impl/Detection.h"

std::vector<Detection> ObjectFilterByPolygon::apply(
    const std::vector<Detection>& detections,
    const std::vector<std::vector<cv::Point>>& includeZones,
    const std::vector<std::vector<cv::Point>>& excludeZones) {
  std::cout << "Filtering " << detections.size() << " detections with "
            << includeZones.size() << " include zones and "
            << excludeZones.size() << " exclude zones" << std::endl;
  std::vector<Detection> filtered;
  for (const auto& det : detections) {
    if (!includeZones.empty()) {
      bool inAny = false;
      for (const auto& zone : includeZones) {
        if (isCenterInZone(det.box, zone)) {
          inAny = true;
          break;
        }
      }
      if (!inAny) continue;
    }
    if (!excludeZones.empty() && isBoxExcluded(det.box, excludeZones)) continue;
    filtered.push_back(det);
  }
  return filtered;
}

bool ObjectFilterByPolygon::isCenterInZone(const cv::Rect& box,
                                           const std::vector<cv::Point>& zone) {
  cv::Point center(box.x + box.width / 2, box.y + box.height / 2);
  return cv::pointPolygonTest(zone, center, false) >= 0;
}

bool ObjectFilterByPolygon::isBoxExcluded(
    const cv::Rect& box,
    const std::vector<std::vector<cv::Point>>& excludeZones) {
  std::vector<cv::Point> boxPoly = {{box.x, box.y},
                                    {box.x + box.width, box.y},
                                    {box.x + box.width, box.y + box.height},
                                    {box.x, box.y + box.height}};
  for (const auto& zone : excludeZones) {
    float intersectionArea =
        cv::intersectConvexConvex(boxPoly, zone, cv::noArray());
    if (intersectionArea > 0) return true;
  }
  return false;
}