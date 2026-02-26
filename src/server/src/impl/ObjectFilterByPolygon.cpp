#include "impl/ObjectFilterByPolygon.h"

#include <opencv2/imgproc.hpp>

#include "impl/Detection.h"

std::vector<Detection> ObjectFilterByPolygon::apply(
    const std::vector<Detection>& detections,
    const std::vector<ImageDetection::Polygon>& polygons, cv::Size imageSize) {
  std::cout << "Filtering " << detections.size() << " detections with "
            << polygons.size() << std::endl;
  std::vector<Detection> filtered;
  std::vector<BinaryPolygon> binaryPolygons;

  for (const auto& it : polygons) {
    binaryPolygons.push_back(createBinaryPolygon(it, imageSize));
  }

  for (const auto& det : detections) {
    if (isDetectionAccepted(det, binaryPolygons)) {
      filtered.push_back(det);
    }
  }
  return filtered;
}

BinaryPolygon ObjectFilterByPolygon::createBinaryPolygon(
    const ImageDetection::Polygon& poly, cv::Size imageSize) {
  BinaryPolygon bp;
  bp.isInclusion = (poly.type() == ImageDetection::PolygonType::INCLUSION);
  bp.threshold = poly.threshold();
  bp.mask = cv::Mat::zeros(imageSize, CV_8UC1);

  std::vector<cv::Point> points;

  for (auto& pt : poly.points()) {
    points.push_back(
        cv::Point(static_cast<int>(pt.x()), static_cast<int>(pt.y())));
  }

  cv::fillPoly(bp.mask, points, cv::Scalar(255));
  bp.boundingBox = cv::boundingRect(points);
  return bp;
}

bool ObjectFilterByPolygon::isDetectionAccepted(
    const Detection& detection, const std::vector<BinaryPolygon>& polygons) {
  for (const auto& poly : polygons) {
    // Быстрая проверка пересечения бокса детекции с охватывающим
    // прямоугольником полигона
    cv::Rect intersect = poly.boundingBox & detection.box;
    if (intersect.area() == 0) continue;  // нет пересечения

    // Проверяем сколько пикселей центра детекции попало в полигон
    cv::Mat roi = poly.mask(detection.box);
    int insideCount = cv::countNonZero(roi);
    float insideRatio = static_cast<float>(insideCount) / detection.box.area();

    if (insideRatio >= poly.threshold) {
      return poly.isInclusion;
    }
  }
  return false;  // не попало ни в один полигон
}