#pragma once
/**
 * @file i_polygon_processor.h
 * @brief Интерфейс предобработки полигонов.
 */
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

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
   */
  virtual void ProcessPolygons(
      std::vector<ImageDetection::Polygon>& polygons) const = 0;
};