#pragma once
/**
 * @file i_drawer.h
 * @brief Интерфейс отрисовки результатов на изображении.
 */
#include <opencv2/core.hpp>
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
struct Polygon;
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
  virtual void DrawDetections(cv::Mat& image,
                              const std::vector<Detection>& detections,
                              std::shared_ptr<IClassMapper> mapper) const = 0;

  /**
   * @brief Рисует контуры полигонов.
   * @param image Изображение.
   * @param polygons Полигоны.
   */
  virtual void DrawPolygons(cv::Mat& image,
                            const std::vector<Polygon>& polygons) const = 0;
};