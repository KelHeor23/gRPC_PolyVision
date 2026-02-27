#pragma once

/**
 * @file Drawer.h
 * @brief Реализация отрисовки детекций и полигонов на изображении.
 */
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IDrawer.h"
#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
class IClassMapper;

/**
 * @class Drawer
 * @brief Рисует рамки детекций и контуры полигонов.
 */
class Drawer : public IDrawer {
 public:
  /**
   * @brief Рисует все детекции на изображении.
   * @param image Изображение.
   * @param detections Вектор обнаружений.
   * @param mapper Маппер классов для получения имён.
   */
  void drawDetections(cv::Mat& image, const std::vector<Detection>& detections,
                      std::shared_ptr<IClassMapper> mapper) override;

  /**
   * @brief Рисует полигоны (зелёным для включения, красным для исключения).
   * @param image Изображение.
   * @param polygons Вектор полигонов в protobuf-формате.
   */
  void drawPolygons(
      cv::Mat& image,
      const std::vector<ImageDetection::Polygon>& polygons) override;
};