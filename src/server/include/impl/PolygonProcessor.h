#pragma once
/**
 * @file PolygonProcessor.h
 * @brief Реализация процессора полигонов (сортировка).
 */
#include "interfaces/IPolygonProcessor.h"

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
  void processPolygons(std::vector<ImageDetection::Polygon>& polygons) override;
};