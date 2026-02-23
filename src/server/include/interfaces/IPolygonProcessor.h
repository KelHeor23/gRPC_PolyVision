#pragma once
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

class IPolygonProcessor {
 public:
  virtual ~IPolygonProcessor() = default;
  virtual void processPolygons(
      std::vector<ImageDetection::Polygon>& polygons) = 0;
};