#pragma once

/**
 * @file ObjectFilterByPolygon.h
 * @brief Фильтрация детекций на основе полигонов.
 */
#include <opencv2/core.hpp>
#include <vector>

#include "interfaces/IObjectFilter.h"
#include "interfaces/IPolygonProcessor.h"

struct Detection;

/**
 * @struct BinaryPolygon
 * @brief Внутреннее представление полигона для быстрой проверки.
 */
struct BinaryPolygon {
  bool isInclusion;  ///< true – зона включения, false – исключения
  float threshold;  ///< доля площади для принадлежности (0..1)
  cv::Mat mask;  ///< бинарная маска полигона
  cv::Rect boundingBox;  ///< охватывающий прямоугольник полигона
};

/**
 * @class ObjectFilterByPolygon
 * @brief Фильтрует детекции, оставляя только те, что удовлетворяют условиям
 * полигонов.
 */
class ObjectFilterByPolygon : public IObjectFilter {
 public:
  /**
   * @brief Конструктор с внедрением процессора полигонов.
   * @param polygonProcessor Объект для предобработки полигонов.
   */
  ObjectFilterByPolygon(std::unique_ptr<IPolygonProcessor> polygonProcessor)
      : polygonProcessor_(std::move(polygonProcessor)) {}

  /**
   * @brief Применяет фильтрацию к детекциям.
   * @param detections Исходный вектор детекций.
   * @param polygons Полигоны (могут быть модифицированы процессором).
   * @param imageSize Размер изображения.
   * @param polygonsName Имя набора полигонов (для кэширования масок).
   * @return Отфильтрованный вектор детекций.
   */
  std::vector<Detection> apply(const std::vector<Detection>& detections,
                               std::vector<ImageDetection::Polygon>& polygons,
                               cv::Size imageSize,
                               const std::string& polygonsName) override;

 private:
  /**
   * @brief Создаёт бинарную маску из protobuf-полигона.
   * @param poly Исходный полигон.
   * @param imageSize Размер изображения.
   * @return Структура BinaryPolygon.
   */
  BinaryPolygon createBinaryPolygon(const ImageDetection::Polygon& poly,
                                    cv::Size imageSize);

  /**
   * @brief Проверяет, удовлетворяет ли одна детекция условиям полигонов.
   * @param detection Детекция.
   * @param polygons Вектор бинарных полигонов.
   * @return true, если детекция принимается.
   */
  bool isDetectionAccepted(const Detection& detection,
                           const std::vector<BinaryPolygon>& polygons);

  std::unique_ptr<IPolygonProcessor>
      polygonProcessor_;  ///< Процессор полигонов
  std::vector<BinaryPolygon> binaryPolygons_;  ///< Кэш бинарных масок
  std::string lastPolygonsName_;  ///< Имя последнего набора полигонов, для
                                  ///< кеширования набора полигонов
};