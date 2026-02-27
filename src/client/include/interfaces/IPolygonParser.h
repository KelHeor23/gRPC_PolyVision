#pragma once
/**
 * @file IPolygonParser.h
 * @brief Интерфейс парсера полигонов из JSON.
 */
#include <boost/json.hpp>
#include <optional>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

/**
 * @interface IPolygonParser
 * @brief Извлекает список полигонов из корневого JSON-объекта.
 */
class IPolygonParser {
 public:
  virtual ~IPolygonParser() = default;
  /**
   * @brief Парсит JSON-объект и возвращает вектор полигонов.
   * @param root Корневой JSON-объект (должен содержать массив "polygons").
   * @return Вектор полигонов или std::nullopt, если данные невалидны.
   */
  virtual std::optional<std::vector<ImageDetection::Polygon>> parse(
      const boost::json::value& root) = 0;
};