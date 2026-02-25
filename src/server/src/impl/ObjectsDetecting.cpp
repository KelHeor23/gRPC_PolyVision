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
                               std::vector<ImageDetection::Polygon>& polygons) {
  std::cout << "Processing image with ObjectsDetecting" << std::endl;
  // Обработка полигонов
  polygonProcessor_->processPolygons(polygons);

  // Если нужно, можно отрисовать полигоны на изображении
  if (drawPolygons_) {
    drawer_->drawPolygons(image, polygons);
  }

  std::vector<std::vector<cv::Point>> includeZones, excludeZones;

  for (const auto& poly : polygons) {
    std::vector<cv::Point> points;
    for (const auto& p : poly.points()) {
      points.emplace_back(static_cast<int>(p.x()), static_cast<int>(p.y()));
    }
    if (points.size() < 3) continue;
    if (poly.type() == ImageDetection::PolygonType::INCLUSION)
      includeZones.push_back(std::move(points));
    else if (poly.type() == ImageDetection::PolygonType::EXCLUSION)
      excludeZones.push_back(std::move(points));
  }

  // Детекция
  auto detections = detector_->detect(image);

  // Фильтрация по зонам
  detections = objectFilter_->apply(detections, includeZones, excludeZones);

  // Отрисовка
  drawer_->drawDetections(image, detections, classMapper_);
}
