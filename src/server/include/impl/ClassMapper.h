#pragma once
#include "interfaces/IClassMapper.h"

class ClassMapper : public IClassMapper {
  std::vector<std::string> classes_;

 public:
  explicit ClassMapper(const std::string& filename);

  std::string getClassName(int classId) const override;

  int size() const override { return classes_.size(); }

  std::vector<int> getAllowedIds(
      const std::vector<std::string>& allowedNames) const override;
};