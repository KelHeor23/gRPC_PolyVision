#include "impl/geometric_filter_by_polygon.h"

#include <algorithm>
#include <ranges>
#include <vector>

#include "impl/detection.h"
#include "impl/polygon_clipper_sutherland_hodgman.h"

std::vector<Detection> GeometricFilterByPolygon::Apply(
    const std::vector<Detection>& detections,
    const std::vector<ImageDetection::Polygon>& polygons, cv::Size image_size,
    const std::string& polygons_name) {
  std::vector<Detection> filtered;
  for (auto& it : detections) {
    auto owner_polygon =
        std::ranges::find_if(polygons, [this, &it](const auto& polygon) {
          return CheckPolygonForOwnership(polygon, it);
        });

    if (owner_polygon != polygons.end() &&
        owner_polygon->type() == ImageDetection::PolygonType::INCLUSION)
      filtered.emplace_back(it);
  }

  return filtered;
}

bool GeometricFilterByPolygon::CheckPolygonForOwnership(
    const ImageDetection::Polygon& polygon, const Detection& detection) {
  std::vector<cv::Point> points_cv;

  points_cv.reserve(polygon.points().size());
  for (auto& point : polygon.points()) {
    points_cv.emplace_back(cv::Point(point.x(), point.y()));
  }

  std::vector<cv::Point> intersection =
      polygon_clipper_->GetInternalPolygon(points_cv, detection.box);

  if (intersection.empty()) return false;

  if (PolygonArea(intersection) >= detection.box.area() * polygon.threshold())
    return true;
  return false;
}

double GeometricFilterByPolygon::PolygonArea(
    const std::vector<cv::Point>& polygon) {
  const size_t n = polygon.size();
  if (n < 3) return 0.0;

  long long area2 = 0;  // удвоенная площадь (целая)
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    area2 += static_cast<long long>(polygon[i].x) * polygon[j].y -
             static_cast<long long>(polygon[j].x) * polygon[i].y;
  }
  return std::abs(area2) * 0.5;
}