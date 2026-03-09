#pragma once
/**
 * @file i_image_processor.h
 * @brief Интерфейс обработчика изображения.
 */
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

/**
 * @interface IImageProcessor
 * @brief Выполняет основную логику обработки (детекцию, фильтрацию, отрисовку).
 */
class IImageProcessor {
 public:
  virtual ~IImageProcessor() = default;
  /**
   * @brief Обрабатывает изображение с учётом полигонов.
   * @param image Изображение (может быть модифицировано).
   * @param polygons Полигоны для фильтрации.
   */
  virtual void Process(cv::Mat& image,
                       std::vector<ImageDetection::Polygon>& polygons) = 0;
};