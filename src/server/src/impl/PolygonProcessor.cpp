#include "impl/PolygonProcessor.h"

#include <algorithm>
#include <ranges>

void PolygonProcessor::processPolygons(
    std::vector<ImageDetection::Polygon>& polygons) {
  std::ranges::sort(polygons, {}, &ImageDetection::Polygon::priority);
}