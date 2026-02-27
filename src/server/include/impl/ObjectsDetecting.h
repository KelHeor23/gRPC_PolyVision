#pragma once
/**
 * @file ObjectsDetecting.h
 * @brief Основной класс обработки изображения: детекция + фильтрация +
 * отрисовка.
 */
#include "Config.h"
#include "interfaces/IImageProcessor.h"

class IClassMapper;
class IDetector;
class IObjectFilter;
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
   * @param config Конфигурация.
   * @param mapper Маппер классов.
   * @param detector Детектор объектов.
   * @param objectFilter Фильтр по полигонам.
   * @param drawer Отрисовщик.
   */
  explicit ObjectsDetecting(const Config& config,
                            std::shared_ptr<IClassMapper> mapper,
                            std::unique_ptr<IDetector> detector,
                            std::unique_ptr<IObjectFilter> objectFilter,
                            std::unique_ptr<IDrawer> drawer);

  /**
   * @brief Выполняет полный цикл обработки изображения.
   * @param image Изображение (модифицируется – на нём рисуются результаты).
   * @param polygons Вектор полигонов.
   * @param fileName Имя набора полигонов (для кэширования).
   */
  void process(cv::Mat& image, std::vector<ImageDetection::Polygon>& polygons,
               const std::string& fileName) override;

  /**
   * @brief Включает/отключает отрисовку полигонов (для отладки).
   * @param draw true – рисовать полигоны false - нет, необходимо для проверки
   */
  void setDrawPolygons(bool draw) { drawPolygons_ = draw; }

 private:
  Config config_;                              ///< Конфигурация
  std::shared_ptr<IClassMapper> classMapper_;  ///< Маппер классов
  std::unique_ptr<IDetector> detector_;  ///< Детектор объектов
  std::unique_ptr<IObjectFilter> objectFilter_;  ///< Фильтр по полигонам
  std::unique_ptr<IDrawer> drawer_;              ///< Отрисовщик
  bool drawPolygons_ = false;  ///< Флаг отрисовки полигонов
};