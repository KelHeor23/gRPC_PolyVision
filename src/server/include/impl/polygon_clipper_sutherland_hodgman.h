#pragma once
/**
 * @file polygon_clipper_sutherland_hodgman.h
 * @brief Класс реализующий алгоритм отсечения полигонов Сазерленда–Ходжмена
 */
#include <vector>

#include "interfaces/i_polygon_clipper.h"

/**
 * @class SutherlandHodgmanClipper
 * @brief Находит внутренний полигон пересечения полигона и прямоугольника
 * используя алгоритм Сазерленда–Ходжмена
 */
class SutherlandHodgmanClipper : public IPolygonClipper {
  /**
   * \enum Edge
   * \brief Сторона окна отсечения для алгоритма Сазерленда–Ходжмена.
   */
  enum Edge {
    LEFT,   ///< Левая грань.
    TOP,    ///< Верхняя грань.
    RIGHT,  ///< Правая грань.
    BOTTOM  ///< Нижняя грань.
  };

  struct RectangleBorders {
    int left;
    int right;
    int top;
    int bottom;
  };

 public:
  /**
   * @brief Возвращает набор точек полигона внутреннего полигона
   * @param polygon весь полигон от которого находжим пересечение
   * @param box бокс/прямоугольник с которым ищем пересечение
   * @return Набор точек вычисленного полигона
   */
  std::vector<ImageDetection::Point> GetInternalPolygon(
      std::vector<ImageDetection::Point> polygon, const cv::Rect& box) override;

 private:
  /**
   * @brief Проверка, находится ли точка внутри относительно заданной стороны
   * @param p првоеряемая точка
   * @param edge проверяемая сторона
   * @return true - точка находится внутри прямоугольника, false - выпадает из
   * него
   */
  bool IsInside(const ImageDetection::Point& p, Edge edge);

  /**
   * @brief Функция находящая точку пересечения отрезка и прямоугольника
   * @param p1 первая точка
   * @param p2 вторая точка
   * @param edge проверяемая сторона
   * @return true - точка находится внутри прямоугольника, false - выпадает из
   * него
   */
  ImageDetection::Point GetIntersectionPoint(const ImageDetection::Point& p1,
                                             const ImageDetection::Point& p2,
                                             Edge edge) const;

 private:
  RectangleBorders borders_;
};