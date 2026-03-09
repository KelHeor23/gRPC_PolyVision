#pragma once
/**
 * @file polygon_processor.h
 * @brief Реализация процессора полигонов (сортировка).
 */
#include "interfaces/i_polygon_processor.h"

/**
 * @class PolygonProcessor
 * @brief Выполняет предобработку полигонов (сортировку по приоритету).
 */
class PolygonProcessor : public IPolygonProcessor {
 public:
  /**
   * @brief Сортирует полигоны по убыванию приоритета, затем по убыванию типа.
   * @param polygons Вектор полигонов (модифицируется).
   */
  void ProcessPolygons(
      std::vector<ImageDetection::Polygon>& polygons) const override;
};