#pragma once
#include <string>
#include <vector>

class IClassMapper {
 public:
  virtual ~IClassMapper() = default;
  virtual std::string getClassName(int classId) const = 0;
  virtual int size() const = 0;
  virtual std::vector<int> getAllowedIds(
      const std::vector<std::string>& allowedNames) const = 0;
};