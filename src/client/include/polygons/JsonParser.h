#pragma once
#include <boost/json.hpp>

#include "interfaces/IJsonParser.h"

class JsonParser : public IJsonParser {
 public:
  std::optional<boost::json::value> parse(const std::string& json) override;
};