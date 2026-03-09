#pragma once
/**
 * @file file_reader.h
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
   * @param file_name Имя файла.
   * @return Содержимое или std::nullopt, если файл не открыт.
   */
  std::optional<std::string> Read(const std::string& file_name) const override;
};