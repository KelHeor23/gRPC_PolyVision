#pragma once
/**
 * @file i_object_filter.h
 * @brief Интерфейс фильтрации детекций по полигонам.
 */
#include "proto/ImageAnalysis.grpc.pb.h"

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
   * @param image_size Размер изображения.
   * @param polygons_name Имя набора полигонов.
   * @return Отфильтрованные детекции.
   */
  virtual std::vector<Detection> Apply(
      const std::vector<Detection>& detections,
      const std::vector<ImageDetection::Polygon>& polygons, cv::Size image_size,
      const std::string& polygons_name) = 0;
};