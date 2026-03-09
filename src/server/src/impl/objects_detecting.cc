#include "impl/objects_detecting.h"

#include "impl/class_mapper.h"
#include "impl/detection.h"
#include "impl/drawer.h"
#include "impl/geometric_filter_by_polygon.h"
#include "impl/polygon.h"
#include "impl/polygon_processor.h"
#include "impl/yolo_detector.h"

ObjectsDetecting::ObjectsDetecting(
    std::shared_ptr<IClassMapper> mapper, std::unique_ptr<IDetector> detector,
    std::unique_ptr<IObjectFilter> object_filter,
    std::unique_ptr<IDrawer> drawer,
    std::unique_ptr<IPolygonProcessor> polygon_processor)
    : class_mapper_(mapper),
      detector_(std::move(detector)),
      object_filter_(std::move(object_filter)),
      drawer_(std::move(drawer)),
      polygon_processor_(std::move(polygon_processor)) {}

void ObjectsDetecting::Process(cv::Mat& image,
                               std::vector<ImageDetection::Polygon>& polygons) {
  // Обработка полигонов
  std::vector<Polygon> sorted_polygons =
      polygon_processor_->ProcessPolygons(polygons);

  // Если нужно, можно отрисовать полигоны на изображении
  if (draw_polygons_) {
    drawer_->DrawPolygons(image, sorted_polygons);
  }

  // Детекция
  auto detections = detector_->Detect(image);

  // Фильтрация по зонам
  detections = object_filter_->Apply(detections, sorted_polygons);

  // Отрисовка
  drawer_->DrawDetections(image, detections, class_mapper_);
}
