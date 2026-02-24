#include "polygons/JsonParser.h"

std::optional<boost::json::value> JsonParser::parse(const std::string& json) {
  try {
    return boost::json::parse(json);
  } catch (...) {
    return std::nullopt;
  }
}