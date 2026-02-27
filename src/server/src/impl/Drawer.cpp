#include "impl/Drawer.h"

#include <format>

#include "impl/ClassMapper.h"
#include "impl/Detection.h"


void Drawer::drawDetections(cv::Mat& image,
                            const std::vector<Detection>& detections,
                            std::shared_ptr<IClassMapper> mapper) {
  std::cout << "Drawing " << detections.size() << " detections" << std::endl;
  for (const auto& det : detections) {
    std::string label = std::format(
        "{}: {:.2f}", mapper->getClassName(det.classId), det.confidence);
    cv::rectangle(image, det.box, cv::Scalar(0, 255, 0), 2);
    cv::putText(image, label, cv::Point(det.box.x, det.box.y - 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
  }
}

void Drawer::drawPolygons(
    cv::Mat& image, const std::vector<ImageDetection::Polygon>& polygons) {
  std::cout << "Drawing " << polygons.size() << " polygons" << std::endl;
  for (const auto& poly : polygons) {
    std::vector<cv::Point> points;
    for (const auto& p : poly.points()) {
      points.emplace_back(static_cast<int>(p.x()), static_cast<int>(p.y()));
    }
    if (points.size() < 3) continue;

    cv::Scalar color = (poly.type() == ImageDetection::PolygonType::INCLUSION)
                           ? cv::Scalar(0, 255, 0)
                           : cv::Scalar(0, 0, 255);
    cv::polylines(image, points, true, color, 2);
  }
}