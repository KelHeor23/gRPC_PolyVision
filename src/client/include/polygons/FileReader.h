#pragma once
#include <fstream>

#include "interfaces/IFileReader.h"

class FileReader : public IFileReader {
 public:
  std::optional<std::string> read(const std::string& filename) override;
};