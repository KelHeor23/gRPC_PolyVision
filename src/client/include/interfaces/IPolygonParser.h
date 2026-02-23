#pragma once
#include <boost/json.hpp>
#include <optional>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

class IPolygonParser {
 public:
  virtual ~IPolygonParser() = default;
  // Парсит корневой JSON-объект и возвращает вектор полигонов или nullopt
  virtual std::optional<std::vector<ImageDetection::Polygon>> parse(
      const boost::json::value& root) = 0;
};