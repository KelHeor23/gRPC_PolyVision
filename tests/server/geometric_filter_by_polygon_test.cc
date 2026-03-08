#include "impl/geometric_filter_by_polygon.h"

#include <gtest/gtest.h>

#include <opencv2/core.hpp>

#include "impl/detection.h"
#include "impl/polygon_clipper_sutherland_hodgman.h"
#include "proto/ImageAnalysis.pb.h"

using ImageDetection::Point;
using ImageDetection::Polygon;
using ImageDetection::PolygonType;

// Вспомогательная функция для создания полигона
Polygon MakePolygon(PolygonType type, int priority, float threshold,
                    const std::vector<std::pair<int, int>>& points) {
  Polygon poly;
  poly.set_type(type);
  poly.set_priority(priority);
  poly.set_threshold(threshold);
  for (const auto& pt : points) {
    auto* p = poly.add_points();
    p->set_x(pt.first);
    p->set_y(pt.second);
  }
  return poly;
}

// Удобное создание прямоугольного полигона
Polygon MakeSquare(int left, int top, int right, int bottom, PolygonType type,
                   int priority, float threshold) {
  return MakePolygon(
      type, priority, threshold,
      {{left, top}, {right, top}, {right, bottom}, {left, bottom}});
}

class GeometricFilterByPolygonTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto clipper = std::make_unique<SutherlandHodgmanClipper>();
    filter_ = std::make_unique<GeometricFilterByPolygon>(std::move(clipper));
  }

  std::unique_ptr<GeometricFilterByPolygon> filter_;
};

TEST_F(GeometricFilterByPolygonTest, NoIntersection) {
  std::vector<Polygon> polygons = {
      MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.5f)};
  Detection det;
  det.box = cv::Rect(20, 20, 5, 5);  // далеко от полигона

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionBelowThreshold) {
  std::vector<Polygon> polygons = {
      MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.8f)};
  Detection det;
  det.box = cv::Rect(5, 5, 10,
                     10);  // площадь 100, пересечение 5x5=25, 25/100=0.25 < 0.8

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionAboveThreshold) {
  std::vector<Polygon> polygons = {
      MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.2f)};
  Detection det;
  det.box = cv::Rect(5, 5, 10, 10);  // 0.25 > 0.2

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].box, det.box);
}

TEST_F(GeometricFilterByPolygonTest, MultiplePolygonsFirstSuitable) {
  std::vector<Polygon> polygons;
  polygons.push_back(MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 1,
                                0.9f));  // не подходит
  polygons.push_back(
      MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 2, 0.1f));  // подходит

  Detection det;
  det.box = cv::Rect(5, 5, 10, 10);  // 0.25

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  ASSERT_EQ(result.size(), 1);  // должна пройти, т.к. второй полигон подходит
}

TEST_F(GeometricFilterByPolygonTest, ExclusionPolygonIgnored) {
  std::vector<Polygon> polygons = {
      MakeSquare(0, 0, 10, 10, PolygonType::EXCLUSION, 1, 0.2f)};
  Detection det;
  det.box = cv::Rect(5, 5, 10, 10);  // подходит по площади, но тип EXCLUSION

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  EXPECT_TRUE(result.empty());  // EXCLUSION не добавляет
}

TEST_F(GeometricFilterByPolygonTest, InclusionAndExclusionMixed) {
  std::vector<Polygon> polygons;
  // Имитируем отфильрованные полигоны
  polygons.push_back(MakeSquare(5, 5, 15, 15, PolygonType::INCLUSION, 2, 0.1f));
  polygons.push_back(MakeSquare(0, 0, 10, 10, PolygonType::EXCLUSION, 1, 0.1f));

  Detection det;
  det.box = cv::Rect(6, 6, 8, 8);  // полностью внутри второго полигона

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  ASSERT_EQ(result.size(), 1);
}

TEST_F(GeometricFilterByPolygonTest, InclusionAndExclusionMixedEqualPriority) {
  std::vector<Polygon> polygons;
  // Имитируем отфильрованные полигоны
  polygons.push_back(MakeSquare(0, 0, 10, 10, PolygonType::EXCLUSION, 2, 0.1f));
  polygons.push_back(MakeSquare(5, 5, 15, 15, PolygonType::INCLUSION, 2, 0.1f));

  Detection det;
  det.box = cv::Rect(6, 6, 8, 8);  // полностью внутри второго полигона

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  ASSERT_EQ(result.size(), 0);
}

TEST_F(GeometricFilterByPolygonTest, NoPolygons) {
  std::vector<Polygon> polygons;
  Detection det;
  det.box = cv::Rect(0, 0, 10, 10);
  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionExactlyThreshold) {
  std::vector<Polygon> polygons = {
      MakeSquare(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.25f)};
  Detection det;
  det.box = cv::Rect(5, 5, 10, 10);  // 25/100 = 0.25

  auto result = filter_->Apply({det}, polygons, cv::Size(100, 100), "test");
  ASSERT_EQ(result.size(), 1);  // должно пройти (>= threshold)
}