#pragma once
/**
 * @file class_mapper.h
 * @brief Реализация маппера классов, загружающего имена из файла.
 */
#include "interfaces/i_class_mapper.h"

/**
 * @class ClassMapper
 * @brief Загружает список имён классов из текстового файла и предоставляет
 * доступ по индексу.
 */
class ClassMapper : public IClassMapper {
 public:
  /**
   * @brief Конструктор, загружающий имена классов из файла.
   * @param file_name Путь к файлу с именами классов (по одному на строку).
   * @throws std::runtime_error если файл не удаётся открыть.
   */
  explicit ClassMapper(const std::string& file_name);

  /**
   * @brief Возвращает имя класса по его идентификатору.
   * @param class_id Идентификатор класса (индекс).
   * @return Имя класса или "unknown", если индекс вне диапазона.
   */
  std::string GetClassName(int class_id) const override;

  /**
   * @brief Количество загруженных классов.
   * @return Размер вектора классов.
   */
  int GetSize() const override { return classes_.size(); }

  /**
   * @brief Преобразует список разрешённых имён в список идентификаторов.
   * @param allowed_names Вектор имён классов.
   * @return Вектор соответствующих индексов (пустые имена игнорируются с
   * предупреждением).
   */
  std::vector<int> GetAllowedIds(
      const std::vector<std::string>& allowed_names) const override;

 private:
  std::vector<std::string>
      classes_;  ///< Вектор имён классов в порядке индексов.
};