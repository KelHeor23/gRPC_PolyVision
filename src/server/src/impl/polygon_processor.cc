#include "impl/polygon_processor.h"

#include <algorithm>
#include <ranges>

void PolygonProcessor::ProcessPolygons(
    std::vector<ImageDetection::Polygon>& polygons) {
  // сортируем по убыванию приоритета, а при равных приоритетах по возрастанию
  // типа
  std::ranges::sort(polygons, [](const auto& a, const auto& b) {
    return a.priority() > b.priority() ||
           (a.priority() == b.priority() && a.type() > b.type());
  });
}