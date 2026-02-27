#pragma once
/**
 * @file ClassMapper.h
 * @brief Реализация маппера классов, загружающего имена из файла.
 */
#include "interfaces/IClassMapper.h"

/**
 * @class ClassMapper
 * @brief Загружает список имён классов из текстового файла и предоставляет
 * доступ по индексу.
 */
class ClassMapper : public IClassMapper {
 public:
  /**
   * @brief Конструктор, загружающий имена классов из файла.
   * @param filename Путь к файлу с именами классов (по одному на строку).
   * @throws std::runtime_error если файл не удаётся открыть.
   */
  explicit ClassMapper(const std::string& filename);

  /**
   * @brief Возвращает имя класса по его идентификатору.
   * @param classId Идентификатор класса (индекс).
   * @return Имя класса или "unknown", если индекс вне диапазона.
   */
  std::string getClassName(int classId) const override;

  /**
   * @brief Количество загруженных классов.
   * @return Размер вектора классов.
   */
  int size() const override { return classes_.size(); }

  /**
   * @brief Преобразует список разрешённых имён в список идентификаторов.
   * @param allowedNames Вектор имён классов.
   * @return Вектор соответствующих индексов (пустые имена игнорируются с
   * предупреждением).
   */
  std::vector<int> getAllowedIds(
      const std::vector<std::string>& allowedNames) const override;

 private:
  std::vector<std::string>
      classes_;  ///< Вектор имён классов в порядке индексов.
};