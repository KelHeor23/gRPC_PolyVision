#pragma once
/**
 * @file IObjectFilter.h
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
   * @param imageSize Размер изображения.
   * @param polygonsName Имя набора полигонов.
   * @return Отфильтрованные детекции.
   */
  virtual std::vector<Detection> apply(
      const std::vector<Detection>& detections,
      std::vector<ImageDetection::Polygon>& polygons, cv::Size imageSize,
      const std::string& polygonsName) = 0;
};