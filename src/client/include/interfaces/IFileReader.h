#pragma once
/**
 * @file IFileReader.h
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
   * @param filename Путь к файлу.
   * @return Содержимое файла или std::nullopt при ошибке.
   */
  virtual std::optional<std::string> read(const std::string& filename) = 0;
};