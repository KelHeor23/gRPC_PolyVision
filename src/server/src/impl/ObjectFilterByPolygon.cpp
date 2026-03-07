#include "impl/ObjectFilterByPolygon.h"

#include <opencv2/imgproc.hpp>

#include "impl/Detection.h"

std::vector<Detection> ObjectFilterByPolygon::Apply(
    const std::vector<Detection>& detections,
    std::vector<ImageDetection::Polygon>& polygons, cv::Size image_size,
    const std::string& polygonsName) {
  // Если пришел нвоый набор полигонов, пересоздаем бинарные маски
  if (last_polygons_name_ != polygonsName) {
    binary_polygons_.clear();
    binary_polygons_.reserve(polygons.size());

    // Обработка полигонов
    polygon_processor_->ProcessPolygons(polygons);

    for (const auto& it : polygons) {
      binary_polygons_.push_back(CreateBinaryPolygon(it, image_size));
    }
    last_polygons_name_ = polygonsName;
  }

  std::vector<Detection> filtered;

  for (const auto& det : detections) {
    if (IsDetectionAccepted(det, binary_polygons_)) {
      filtered.push_back(det);
    }
  }
  return filtered;
}

BinaryPolygon ObjectFilterByPolygon::CreateBinaryPolygon(
    const ImageDetection::Polygon& poly, cv::Size image_size) {
  BinaryPolygon bp;
  bp.is_inclusion = (poly.type() == ImageDetection::PolygonType::INCLUSION);
  bp.threshold = poly.threshold();
  bp.mask = cv::Mat::zeros(image_size, CV_8UC1);

  std::vector<cv::Point> points;

  for (auto& pt : poly.points()) {
    points.push_back(
        cv::Point(static_cast<int>(pt.x()), static_cast<int>(pt.y())));
  }

  cv::fillPoly(bp.mask, points, cv::Scalar(255));
  bp.bounding_box = cv::boundingRect(points);
  return bp;
}

bool ObjectFilterByPolygon::IsDetectionAccepted(
    const Detection& detection, const std::vector<BinaryPolygon>& polygons) {
  for (const auto& poly : polygons) {
    // Быстрая проверка пересечения бокса детекции с охватывающим
    // прямоугольником полигона
    cv::Rect intersect = poly.bounding_box & detection.box;
    if (intersect.area() == 0) continue;  // нет пересечения

    // Проверяем долю площади бокса детекции, которая попадает в полигон
    cv::Mat roi = poly.mask(detection.box);
    int insideCount = cv::countNonZero(roi);
    float inside_ratio = static_cast<float>(insideCount) / detection.box.area();

    if (inside_ratio >= poly.threshold) {
      return poly.is_inclusion;
    }
  }
  return false;  // не попало ни в один полигон
}