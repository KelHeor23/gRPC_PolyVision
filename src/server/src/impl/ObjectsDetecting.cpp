#include "impl/ObjectsDetecting.h"

#include "impl/ClassMapper.h"
#include "impl/Detection.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/YoloDetector.h"

ObjectsDetecting::ObjectsDetecting(std::shared_ptr<IClassMapper> mapper,
                                   std::unique_ptr<IDetector> detector,
                                   std::unique_ptr<IObjectFilter> objectFilter,
                                   std::unique_ptr<IDrawer> drawer)
    : classMapper_(mapper),
      detector_(std::move(detector)),
      objectFilter_(std::move(objectFilter)),
      drawer_(std::move(drawer)) {}

void ObjectsDetecting::process(cv::Mat& image,
                               std::vector<ImageDetection::Polygon>& polygons,
                               const std::string& polygonsName) {
  // Если нужно, можно отрисовать полигоны на изображении
  if (drawPolygons_) {
    drawer_->drawPolygons(image, polygons);
  }

  // Детекция
  auto detections = detector_->detect(image);

  // Фильтрация по зонам
  detections =
      objectFilter_->apply(detections, polygons, image.size(), polygonsName);

  // Отрисовка
  drawer_->drawDetections(image, detections, classMapper_);
}
