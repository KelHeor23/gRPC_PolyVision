#pragma once
/**
 * @file IPolygonProcessor.h
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
  virtual void processPolygons(
      std::vector<ImageDetection::Polygon>& polygons) = 0;
};