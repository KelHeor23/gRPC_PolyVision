#pragma once
/**
 * @file polygon_processor.h
 * @brief Реализация процессора полигонов (сортировка).
 */
#include "impl/polygon.h"
#include "interfaces/i_polygon_processor.h"

class IClassMapper;
/**
 * @class PolygonProcessor
 * @brief Выполняет предобработку полигонов (сортировку по приоритету).
 */
class PolygonProcessor : public IPolygonProcessor {
 public:
  /**
   * @brief Конструктор класса преобразующего полигоны
   * @param mapper Маппер классов.
   */
  PolygonProcessor(std::shared_ptr<IClassMapper> mapper) : mapper_(mapper) {}

  /**
   * @brief Сортирует полигоны, добавляет к полигонам список детектиремых
   * объектов
   * @param polygons Вектор полигонов (модифицируется).
   * @return Вектор отсортированных полигонов, со таблицей детектируемых
   * объектов.
   */
  std::vector<Polygon> ProcessPolygons(
      std::vector<ImageDetection::Polygon> polygons) const override;

 private:
  /**
   * @brief Сортирует полигоны по убыванию приоритета, затем по убыванию типа.
   * @param polygons Вектор полигонов (модифицируется).
   */
  void sort(std::vector<Polygon>& polygons) const;

 private:
  std::shared_ptr<IClassMapper>
      mapper_;  ///< Класс работающий со списком детектируемых объектов в модели
};