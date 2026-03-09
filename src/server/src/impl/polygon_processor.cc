#include "impl/polygon_processor.h"

#include <algorithm>
#include <ranges>

#include "impl/class_mapper.h"
#include "impl/polygon.h"

std::vector<Polygon> PolygonProcessor::ProcessPolygons(
    std::vector<ImageDetection::Polygon> polygons) const {
  std::vector<Polygon> result;
  result.reserve(polygons.size());
  for (auto& poly : polygons) {
    std::vector<std::string> class_names_vec(poly.class_names().begin(),
                                             poly.class_names().end());
    result.push_back(
        {std::move(poly), mapper_->GetAllowedIds(class_names_vec)});
  }
  sort(result);
  return result;
}

void PolygonProcessor::sort(std::vector<Polygon>& polygons) const {
  // сортируем по убыванию приоритета, а при равных приоритетах по убыванию
  // типа
  std::ranges::sort(polygons, [](const auto& a, const auto& b) {
    return a.polygon.priority() > b.polygon.priority() ||
           (a.polygon.priority() == b.polygon.priority() &&
            a.polygon.type() > b.polygon.type());
  });
}