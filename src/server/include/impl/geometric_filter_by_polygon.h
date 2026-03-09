#pragma once

/**
 * @file geometric_filter_by_polygon.h
 * @brief Фильтрация детекций на основе полигонов, используя геометрический
 * подход
 */
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/i_object_filter.h"
#include "interfaces/i_polygon_clipper.h"
#include "proto/ImageAnalysis.grpc.pb.h"

struct Detection;
class IClassMapper;

/**
 * @class GeometricFilterByPolygon
 * @brief Фильтрует детекции используя геометрический подходи, оставляя только
 * те, что удовлетворяют условиям полигонов.
 */
class GeometricFilterByPolygon : public IObjectFilter {
 public:
  /**
   * @brief Конструктор с внедрением процессора полигонов.
   * @param polygon_clipper Объект для порлучения внутренних полигонов
   * @param mapper Маппер классов.
   */
  GeometricFilterByPolygon(std::unique_ptr<IPolygonClipper> polygon_clipper,
                           std::shared_ptr<IClassMapper> mapper)
      : polygon_clipper_(std::move(polygon_clipper)), mapper_(mapper) {}

  /**
   * @brief Применяет фильтрацию к детекциям.
   * @param detections Исходный вектор детекций.
   * @param polygons Полигоны (могут быть модифицированы процессором).
   * @return Отфильтрованный вектор детекций.
   */
  std::vector<Detection> Apply(
      const std::vector<Detection>& detections,
      const std::vector<ImageDetection::Polygon>& polygons) override;

 private:
  /**
   * @brief Проверяет является ли полигон главным определяющим. (Полигон должен
   * пересекаться с задетектированным изображением и площадь пересечения должна
   * быть >= изображдения * Polygon::threshold)
   * @param polygon Полигоны.
   * @param detection Размер изображения.
   * @return Отфильтрованный вектор детекций.
   */
  bool CheckPolygonForOwnership(const ImageDetection::Polygon& polygon,
                                const Detection& detection);

  /**
   * @brief Вычисляет площадь полигона по формуле Гаусса (формула шнурка).
   * Вершины должны быть упорядочены (по или против часовой стрелки).
   * Для пустого или вырожденного полигона (менее 3 точек) возвращает 0.
   *
   * @param polygon вектор вершин типа ImageDetection::Point
   * @return площадь полигона (double)
   */
  double PolygonArea(const std::vector<ImageDetection::Point>& polygon) const;

 private:
  std::unique_ptr<IPolygonClipper> polygon_clipper_;  ///< обрезальщик полигонов
  std::shared_ptr<IClassMapper>
      mapper_;  ///< Класс работающий со списком детектируемых объектов в модели
};