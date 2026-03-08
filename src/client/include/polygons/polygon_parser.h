#pragma once
/**
 * @file polygon_parser.h
 * @brief Реализация IPolygonParser для извлечения полигонов и метаданных из
 * JSON.
 */
#include "interfaces/i_polygon_parser.h"

/**
 * @class PolygonParser
 * @brief Разбирает JSON-объект в protobuf-сообщение PolygonList.
 */
class PolygonParser : public IPolygonParser {
 public:
  /**
   * @brief Основной метод парсинга.
   * @param root Корневой JSON-объект.
   * @return PolygonList или nullopt, если данные некорректны.
   */
  std::optional<ImageDetection::PolygonList> Parse(
      const boost::json::value& root) override;

 private:
  /**
   * @brief Парсит один элемент массива полигонов.
   * @param val JSON-значение.
   * @param index Индекс полигона для диагностики.
   * @return Полигон или nullopt при ошибке.
   */
  std::optional<ImageDetection::Polygon> ParseSinglePolygon(
      const boost::json::value& val, std::size_t index);

  /**
   * @brief Парсит массив имён классов.
   * @param val JSON-значение (ожидается массив строк).
   * @return Вектор строк или nullopt при ошибке.
   */
  std::optional<google::protobuf::RepeatedPtrField<std::string>>
  ParseClassNames(const boost::json::value& val);
};