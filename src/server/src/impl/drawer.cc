#include "impl/drawer.h"

#include <format>

#include "impl/class_mapper.h"
#include "impl/detection.h"

void Drawer::DrawDetections(cv::Mat& image,
                            const std::vector<Detection>& detections,
                            std::shared_ptr<IClassMapper> mapper) const {
  for (const auto& det : detections) {
    std::string label = std::format(
        "{}: {:.2f}", mapper->GetClassName(det.class_id), det.confidence);
    cv::rectangle(image, det.box, cv::Scalar(0, 255, 0), 2);
    cv::putText(image, label, cv::Point(det.box.x, det.box.y - 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
  }
}

void Drawer::DrawPolygons(
    cv::Mat& image,
    const std::vector<ImageDetection::Polygon>& polygons) const {
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