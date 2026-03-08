#pragma once

#include <vector>

/**
 * @file i_polygon_clipper.h.h
 * @brief Интерфейс определения полигона пересечения.
 */
#include <opencv2/opencv.hpp>
/**
 * @interface IPolygonClipper
 * @brief Находит внутренний полигон пересечения двух фигур
 */
class IPolygonClipper {
 public:
  virtual ~IPolygonClipper() = default;
  /**
   * @brief Возвращает набор точек полигона внутреннего полигона
   * @param polygon полигон от которого находжим пересечение
   * @param box бокс/прямоугольник с которым ищем пересечение
   * @return Набор точек вычисленного полигона
   */
  virtual std::vector<cv::Point> GetInternalPolygon(
      std::vector<cv::Point> polygon, const cv::Rect& box) = 0;
};