#pragma once
/**
 * @file IDrawer.h
 * @brief Интерфейс отрисовки результатов на изображении.
 */
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
class IClassMapper;

/**
 * @interface IDrawer
 * @brief Рисует детекции и полигоны.
 */
class IDrawer {
 public:
  virtual ~IDrawer() = default;
  /**
   * @brief Рисует рамки обнаруженных объектов и подписи.
   * @param image Изображение.
   * @param detections Детекции.
   * @param mapper Маппер для получения имён классов.
   */
  virtual void drawDetections(cv::Mat& image,
                              const std::vector<Detection>& detections,
                              std::shared_ptr<IClassMapper> mapper) = 0;

  /**
   * @brief Рисует контуры полигонов.
   * @param image Изображение.
   * @param polygons Полигоны.
   */
  virtual void drawPolygons(
      cv::Mat& image, const std::vector<ImageDetection::Polygon>& polygons) = 0;
};