#include "impl/drawer.h"

#include <format>
#include <ranges>

#include "impl/class_mapper.h"
#include "impl/detection.h"
#include "impl/polygon.h"

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

void Drawer::DrawPolygons(cv::Mat& image,
                          const std::vector<Polygon>& polygons) const {
  for (const auto& poly : polygons) {
    auto points_view =
        poly.polygon.points() | std::views::transform([](const auto& p) {
          return cv::Point(p.x(), p.y());
        });
    std::vector<cv::Point> points(points_view.begin(), points_view.end());

    if (points.size() < 3) continue;

    cv::Scalar color =
        (poly.polygon.type() == ImageDetection::PolygonType::INCLUSION)
            ? cv::Scalar(0, 255, 0)
            : cv::Scalar(0, 0, 255);
    cv::polylines(image, points, true, color, 2);
  }
}