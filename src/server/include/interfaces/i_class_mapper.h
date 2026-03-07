#pragma once
/**
 * @file i_class_mapper.h
 * @brief Интерфейс для преобразования идентификатора класса в его имя.
 */
#include <string>
#include <vector>

/**
 * @interface IClassMapper
 * @brief Абстрактный маппер классов.
 */
class IClassMapper {
 public:
  virtual ~IClassMapper() = default;
  /**
   * @brief Возвращает имя класса по идентификатору.
   * @param classId Идентификатор класса.
   * @return Имя класса.
   */
  virtual std::string GetClassName(int classId) const = 0;
  /**
   * @brief Количество известных классов.
   * @return Размер.
   */
  virtual int GetSize() const = 0;
  /**
   * @brief Преобразует список имён в список идентификаторов.
   * @param allowedNames Имена классов.
   * @return Вектор соответствующих идентификаторов.
   */
  virtual std::vector<int> GetAllowedIds(
      const std::vector<std::string>& allowedNames) const = 0;
};