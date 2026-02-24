#pragma once
#include <optional>
#include <string>

class IFileReader {
 public:
  virtual ~IFileReader() = default;
  virtual std::optional<std::string> read(const std::string& filename) = 0;
};