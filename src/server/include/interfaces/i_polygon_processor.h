#pragma once
/**
 * @file i_polygon_processor.h
 * @brief Интерфейс предобработки полигонов.
 */
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

struct Polygon;
/**
 * @interface IPolygonProcessor
 * @brief Выполняет операции над полигонами перед их использованием.
 */
class IPolygonProcessor {
 public:
  virtual ~IPolygonProcessor() = default;
  /**
   * @brief Обрабатывает полигоны (например, сортирует).
   * @param polygons Вектор полигонов (модифицируется).
   * @return Вектор отсортированных полигонов, со таблицей детектируемых
   * объектов.
   */
  virtual std::vector<Polygon> ProcessPolygons(
      std::vector<ImageDetection::Polygon> polygons) const = 0;
};