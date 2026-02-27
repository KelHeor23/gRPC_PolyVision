#pragma once
/**
 * @file PolygonParser.h
 * @brief Реализация IPolygonParser для извлечения полигонов из JSON.
 */
#include "interfaces/IPolygonParser.h"

/**
 * @class PolygonParser
 * @brief Разбирает JSON-массив "polygons" в protobuf-сообщения Polygon.
 */
class PolygonParser : public IPolygonParser {
 public:
  /**
   * @brief Основной метод парсинга.
   * @param root Корневой JSON-объект.
   * @return Вектор полигонов или nullopt, если данные некорректны.
   */
  std::optional<std::vector<ImageDetection::Polygon>> parse(
      const boost::json::value& root) override;

 private:
  /**
   * @brief Парсит один элемент массива полигонов.
   * @param val JSON-значение.
   * @param index Индекс полигона для диагностики.
   * @return Полигон или nullopt при ошибке.
   */
  std::optional<ImageDetection::Polygon> parseSinglePolygon(
      const boost::json::value& val, std::size_t index);
};