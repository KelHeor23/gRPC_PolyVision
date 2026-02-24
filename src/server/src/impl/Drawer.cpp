#include "impl/Drawer.h"

#include <opencv2/imgproc.hpp>

void Drawer::draw(cv::Mat& image,
                  const std::vector<ImageDetection::Polygon>& polygons) {
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
