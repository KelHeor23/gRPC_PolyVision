#include "impl/ClassMapper.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

ClassMapper::ClassMapper(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open classes file: " + filename);
  }
  std::string line;
  while (std::getline(file, line)) {
    // Удаляем пробельные символы
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    line.erase(0, line.find_first_not_of(" \n\r\t"));
    classes_.push_back(line);
  }
}

std::string ClassMapper::getClassName(int classId) const {
  if (classId >= 0 && classId < static_cast<int>(classes_.size()))
    return classes_[classId];
  return "unknown";
}

std::vector<int> ClassMapper::getAllowedIds(
    const std::vector<std::string>& allowedNames) const {
  std::vector<int> ids;
  for (const auto& name : allowedNames) {
    auto it = std::find(classes_.begin(), classes_.end(), name);
    if (it != classes_.end())
      ids.push_back(it - classes_.begin());
    else
      std::cerr << "Warning: Class '" << name << "' not found, ignored.\n";
  }
  return ids;
}