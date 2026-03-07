#pragma once
/**
 * @file FileReader.h
 * @brief Реализация IFileReader с использованием std::ifstream.
 */
#include <fstream>

#include "interfaces/i_file_reader.h"
/**
 * @class FileReader
 * @brief Читает файл целиком в строку.
 */
class FileReader : public IFileReader {
 public:
  /**
   * @brief Открывает файл и читает его содержимое.
   * @param filename Имя файла.
   * @return Содержимое или std::nullopt, если файл не открыт.
   */
  std::optional<std::string> Read(const std::string& filename) override;
};