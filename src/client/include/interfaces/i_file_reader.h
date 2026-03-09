#pragma once
/**
 * @file i_file_reader.h
 * @brief Интерфейс для чтения содержимого файла.
 */
#include <optional>
#include <string>

/**
 * @interface IFileReader
 * @brief Позволяет читать текстовые файлы.
 */
class IFileReader {
 public:
  virtual ~IFileReader() = default;
  /**
   * @brief Читает файл и возвращает его содержимое.
   * @param file_name Путь к файлу.
   * @return Содержимое файла или std::nullopt при ошибке.
   */
  virtual std::optional<std::string> Read(
      const std::string& file_name) const = 0;
};