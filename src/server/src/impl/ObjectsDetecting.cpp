#include "impl/ObjectsDetecting.h"

#include "impl/ClassMapper.h"
#include "impl/Detection.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/PolygonProcessor.h"
#include "impl/YoloDetector.h"

ObjectsDetecting::ObjectsDetecting(
    const Config& config, std::shared_ptr<IClassMapper> mapper,
    std::unique_ptr<IDetector> detector,
    std::unique_ptr<IPolygonProcessor> polygonProcessor,
    std::unique_ptr<IObjectFilter> objectFilter,
    std::unique_ptr<IDrawer> drawer)
    : config_(config),
      classMapper_(mapper),
      detector_(std::move(detector)),
      polygonProcessor_(std::move(polygonProcessor)),
      objectFilter_(std::move(objectFilter)),
      drawer_(std::move(drawer)) {}

void ObjectsDetecting::process(cv::Mat& image,
                               std::vector<ImageDetection::Polygon>& polygons,
                               const std::string& polygonsName) {
  // Обработка полигонов
  polygonProcessor_->processPolygons(polygons);

  // Если нужно, можно отрисовать полигоны на изображении
  if (drawPolygons_) {
    drawer_->drawPolygons(image, polygons);
  }

  // Детекция
  auto detections = detector_->detect(image);

  // Фильтрация по зонам
  detections = objectFilter_->apply(detections, polygons, image.size());

  // Отрисовка
  drawer_->drawDetections(image, detections, classMapper_);
}
