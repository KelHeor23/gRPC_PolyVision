#pragma once
#include "interfaces/IPolygonParser.h"

class PolygonParser : public IPolygonParser {
 public:
  std::optional<std::vector<ImageDetection::Polygon>> parse(
      const boost::json::value& root) override;

 private:
  std::optional<ImageDetection::Polygon> parseSinglePolygon(
      const boost::json::value& val, std::size_t index);
};