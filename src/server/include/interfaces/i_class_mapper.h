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
   * @param class_id Идентификатор класса.
   * @return Имя класса.
   */
  virtual std::string GetClassName(int class_id) const = 0;
  /**
   * @brief Количество известных классов.
   * @return Размер.
   */
  virtual int GetSize() const = 0;
  /**
   * @brief Преобразует список имён в список идентификаторов.
   * @param allowed_names Имена классов.
   * @return Вектор соответствующих идентификаторов.  Вектор работает быстрее
   * при небольшом колчиестве данных, если количество детектируемых классов
   * будет увеличено, то лучше перейти на unordered_set
   */
  virtual std::vector<int> GetAllowedIds(
      const std::vector<std::string>& allowed_names) const = 0;
};