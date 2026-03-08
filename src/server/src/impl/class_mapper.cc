#include "impl/class_mapper.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

ClassMapper::ClassMapper(const std::string& file_name) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open classes file: " + file_name);
  }
  std::string line;
  while (std::getline(file, line)) {
    // Удаляем пробельные символы
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    line.erase(0, line.find_first_not_of(" \n\r\t"));
    classes_.push_back(line);
  }
}

std::string ClassMapper::GetClassName(int class_id) const {
  if (class_id >= 0 && class_id < static_cast<int>(classes_.size()))
    return classes_[class_id];
  return "unknown";
}

std::vector<int> ClassMapper::GetAllowedIds(
    const std::vector<std::string>& allowed_names) const {
  std::vector<int> ids;
  for (const auto& name : allowed_names) {
    auto it = std::find(classes_.begin(), classes_.end(), name);
    if (it != classes_.end())
      ids.push_back(it - classes_.begin());
    else
      std::cerr << "Warning: Class '" << name << "' not found, ignored.\n";
  }
  return ids;
}