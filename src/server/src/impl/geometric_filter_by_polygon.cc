#include "impl/geometric_filter_by_polygon.h"

#include <algorithm>
#include <ranges>
#include <vector>

#include "impl/class_mapper.h"
#include "impl/detection.h"
#include "impl/polygon_clipper_sutherland_hodgman.h"

std::vector<Detection> GeometricFilterByPolygon::Apply(
    const std::vector<Detection>& detections,
    const std::vector<ImageDetection::Polygon>& polygons) {
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
  std::vector<std::string> class_names_vec(polygon.class_names().begin(),
                                           polygon.class_names().end());
  std::vector<int> allowed_id = mapper_->GetAllowedIds(class_names_vec);

  if (std::find(allowed_id.begin(), allowed_id.end(), detection.class_id) ==
      allowed_id.end())
    return false;

  const auto& points_repeated = polygon.points();
  std::vector<ImageDetection::Point> points_vec(points_repeated.begin(),
                                                points_repeated.end());

  std::vector<ImageDetection::Point> intersection =
      polygon_clipper_->GetInternalPolygon(std::move(points_vec),
                                           detection.box);

  if (intersection.empty()) return false;

  if (PolygonArea(intersection) >= detection.box.area() * polygon.threshold())
    return true;
  return false;
}

double GeometricFilterByPolygon::PolygonArea(
    const std::vector<ImageDetection::Point>& polygon) const {
  const size_t n = polygon.size();
  if (n < 3) return 0.0;

  int64_t area2 = 0;  // удвоенная площадь (целая)
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    area2 += static_cast<int64_t>(polygon[i].x()) * polygon[j].y() -
             static_cast<int64_t>(polygon[j].x()) * polygon[i].y();
  }
  return std::abs(area2) * 0.5;
}