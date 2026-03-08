#pragma once
/**
 * @file objects_detecting.h
 * @brief Основной класс обработки изображения: детекция + фильтрация +
 * отрисовка.
 */
#include "interfaces/i_image_processor.h"

class IClassMapper;
class IDetector;
class IObjectFilter;
class IPolygonProcessor;
class IDrawer;

/**
 * @class ObjectsDetecting
 * @brief Координирует процесс детекции объектов, фильтрации по полигонам и
 * отрисовки.
 */
class ObjectsDetecting : public IImageProcessor {
 public:
  /**
   * @brief Конструктор с внедрением всех зависимостей.
   * @param mapper Маппер классов.
   * @param detector Детектор объектов.
   * @param objectFilter Фильтр по полигонам.
   * @param drawer Отрисовщик.
   * @param polygonProcessor Объект для предобработки полигонов.
   */
  explicit ObjectsDetecting(
      std::shared_ptr<IClassMapper> mapper, std::unique_ptr<IDetector> detector,
      std::unique_ptr<IObjectFilter> objectFilter,
      std::unique_ptr<IDrawer> drawer,
      std::unique_ptr<IPolygonProcessor> polygonProcessor);

  /**
   * @brief Выполняет полный цикл обработки изображения.
   * @param image Изображение (модифицируется – на нём рисуются результаты).
   * @param polygons Вектор полигонов.
   * @param fileName Имя набора полигонов (для кэширования).
   */
  void Process(cv::Mat& image, std::vector<ImageDetection::Polygon>& polygons,
               const std::string& fileName) override;

  /**
   * @brief Включает/отключает отрисовку полигонов (для отладки).
   * @param draw true – рисовать полигоны false - нет, необходимо для проверки
   */
  void SetDrawPolygons(bool draw) { draw_polygons_ = draw; }

 private:
  std::shared_ptr<IClassMapper> class_mapper_;  ///< Маппер классов
  std::unique_ptr<IDetector> detector_;  ///< Детектор объектов
  std::unique_ptr<IObjectFilter> object_filter_;  ///< Фильтр по полигонам
  std::unique_ptr<IDrawer> drawer_;               ///< Отрисовщик
  std::unique_ptr<IPolygonProcessor>
      polygon_processor_;       ///< Процессор полигонов
  bool draw_polygons_ = false;  ///< Флаг отрисовки полигонов
};