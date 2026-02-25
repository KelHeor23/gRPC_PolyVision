#include "impl/PolygonProcessor.h"

#include <algorithm>
#include <ranges>

void PolygonProcessor::processPolygons(
    std::vector<ImageDetection::Polygon>& polygons) {
  std::cout << "Processing " << polygons.size() << " polygons" << std::endl;
  std::ranges::sort(polygons, {}, &ImageDetection::Polygon::priority);
}