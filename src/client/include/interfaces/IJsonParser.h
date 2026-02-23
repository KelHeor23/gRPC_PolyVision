#pragma once
#include <boost/json.hpp>
#include <optional>
#include <string>

class IJsonParser {
 public:
  virtual ~IJsonParser() = default;
  virtual std::optional<boost::json::value> parse(const std::string& json) = 0;
};