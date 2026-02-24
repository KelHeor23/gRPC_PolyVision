#pragma once

#include "interfaces/IPolygonProcessor.h"

class PolygonProcessor : public IPolygonProcessor {
 public:
  void processPolygons(std::vector<ImageDetection::Polygon>& polygons) override;
};