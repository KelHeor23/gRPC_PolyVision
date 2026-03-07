#include "impl/ObjectsDetecting.h"

#include "impl/ClassMapper.h"
#include "impl/Detection.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/YoloDetector.h"

ObjectsDetecting::ObjectsDetecting(std::shared_ptr<IClassMapper> mapper,
                                   std::unique_ptr<IDetector> detector,
                                   std::unique_ptr<IObjectFilter> object_filter,
                                   std::unique_ptr<IDrawer> drawer)
    : class_mapper_(mapper),
      detector_(std::move(detector)),
      object_filter_(std::move(object_filter)),
      drawer_(std::move(drawer)) {}

void ObjectsDetecting::Process(cv::Mat& image,
                               std::vector<ImageDetection::Polygon>& polygons,
                               const std::string& polygons_name) {
  // Если нужно, можно отрисовать полигоны на изображении
  if (draw_polygons_) {
    drawer_->DrawPolygons(image, polygons);
  }

  // Детекция
  auto detections = detector_->Detect(image);

  // Фильтрация по зонам
  detections =
      object_filter_->Apply(detections, polygons, image.size(), polygons_name);

  // Отрисовка
  drawer_->DrawDetections(image, detections, class_mapper_);
}
