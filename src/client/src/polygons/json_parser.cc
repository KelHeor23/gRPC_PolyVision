#include "polygons/json_parser.h"

std::optional<boost::json::value> JsonParser::Parse(const std::string& json) {
  try {
    return boost::json::parse(json);
  } catch (...) {
    return std::nullopt;
  }
}