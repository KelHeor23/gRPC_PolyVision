#pragma once
/**
 * @file i_object_filter.h
 * @brief Интерфейс фильтрации детекций по полигонам.
 */
#include "proto/ImageAnalysis.grpc.pb.h"

struct Polygon;
struct Detection;
/**
 * @interface IObjectFilter
 * @brief Фильтрует список обнаружений в соответствии с заданными полигонами.
 */
class IObjectFilter {
 public:
  virtual ~IObjectFilter() = default;
  /**
   * @brief Применяет фильтрацию.
   * @param detections Исходные детекции.
   * @param polygons Полигоны
   * @return Отфильтрованные детекции.
   */
  virtual std::vector<Detection> Apply(
      const std::vector<Detection>& detections,
      const std::vector<Polygon>& polygons) = 0;
};