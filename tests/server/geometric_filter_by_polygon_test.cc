#include "impl/geometric_filter_by_polygon.h"

#include <gtest/gtest.h>

#include <memory>
#include <opencv2/core.hpp>
#include <vector>

#include "impl/detection.h"
#include "impl/polygon.h"
#include "impl/polygon_clipper_sutherland_hodgman.h"
#include "proto/ImageAnalysis.pb.h"

using ImageDetection::Point;
using ImageDetection::PolygonType;

// Вспомогательная функция для создания protobuf-полигона
ImageDetection::Polygon MakeProtoPolygon(
    PolygonType type, int priority, float threshold,
    const std::vector<std::pair<int, int>>& points,
    const std::vector<std::string>& class_names = {"test"}) {
  ImageDetection::Polygon poly;
  poly.set_type(type);
  poly.set_priority(priority);
  poly.set_threshold(threshold);
  for (const auto& pt : points) {
    auto* p = poly.add_points();
    p->set_x(pt.first);
    p->set_y(pt.second);
  }
  for (const auto& name : class_names) {
    poly.add_class_names(name);
  }
  return poly;
}

// Создание прямоугольного protobuf-полигона
ImageDetection::Polygon MakeSquareProto(int left, int top, int right,
                                        int bottom, PolygonType type,
                                        int priority, float threshold) {
  return MakeProtoPolygon(
      type, priority, threshold,
      {{left, top}, {right, top}, {right, bottom}, {left, bottom}});
}

// Создание нашей структуры Polygon с allowed_id
Polygon MakePolygon(const ImageDetection::Polygon& proto,
                    const std::vector<int>& allowed_ids) {
  return Polygon{proto, allowed_ids};
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
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.5f);
  std::vector<Polygon> polygons = {MakePolygon(proto, {0})};

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(20, 20, 5, 5);

  auto result = filter_->Apply({det}, polygons);
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionBelowThreshold) {
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.8f);
  std::vector<Polygon> polygons = {MakePolygon(proto, {0})};

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(5, 5, 10, 10);

  auto result = filter_->Apply({det}, polygons);
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionAboveThreshold) {
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.2f);
  std::vector<Polygon> polygons = {MakePolygon(proto, {0})};

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(5, 5, 10, 10);

  auto result = filter_->Apply({det}, polygons);
  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].box, det.box);
}

TEST_F(GeometricFilterByPolygonTest, ClassIdNotAllowed) {
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.2f);
  std::vector<Polygon> polygons = {
      MakePolygon(proto, {1})};  // разрешён только класс 1

  Detection det;
  det.class_id = 0;  // детекция класса 0
  det.box = cv::Rect(5, 5, 10, 10);

  auto result = filter_->Apply({det}, polygons);
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, MultiplePolygonsFirstSuitable) {
  std::vector<Polygon> polygons;
  polygons.push_back(MakePolygon(
      MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.9f), {0}));
  polygons.push_back(MakePolygon(
      MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 2, 0.1f), {0}));

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(5, 5, 10, 10);

  auto result = filter_->Apply({det}, polygons);
  ASSERT_EQ(result.size(), 1);
  // Первый полигон (priority 2) должен сработать
}

TEST_F(GeometricFilterByPolygonTest, ExclusionPolygonIgnored) {
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::EXCLUSION, 1, 0.2f);
  std::vector<Polygon> polygons = {MakePolygon(proto, {0})};

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(5, 5, 10, 10);

  auto result = filter_->Apply({det}, polygons);
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, InclusionAndExclusionMixed) {
  std::vector<Polygon> polygons;
  polygons.push_back(MakePolygon(
      MakeSquareProto(5, 5, 15, 15, PolygonType::INCLUSION, 2, 0.1f), {0}));
  polygons.push_back(MakePolygon(
      MakeSquareProto(0, 0, 10, 10, PolygonType::EXCLUSION, 1, 0.1f), {0}));

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(6, 6, 8, 8);

  auto result = filter_->Apply({det}, polygons);
  ASSERT_EQ(result.size(), 1);  // INCLUSION с более высоким приоритетом
}

TEST_F(GeometricFilterByPolygonTest, InclusionAndExclusionMixedEqualPriority) {
  std::vector<Polygon> polygons;
  polygons.push_back(MakePolygon(
      MakeSquareProto(0, 0, 10, 10, PolygonType::EXCLUSION, 2, 0.1f), {0}));
  polygons.push_back(MakePolygon(
      MakeSquareProto(5, 5, 15, 15, PolygonType::INCLUSION, 2, 0.1f), {0}));

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(6, 6, 8, 8);

  auto result = filter_->Apply({det}, polygons);
  ASSERT_EQ(result.size(),
            0);  // EXCLUSION идёт раньше из-за type() > (EXCLUSION > INCLUSION)
}

TEST_F(GeometricFilterByPolygonTest, NoPolygons) {
  std::vector<Polygon> polygons;
  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(0, 0, 10, 10);
  auto result = filter_->Apply({det}, polygons);
  EXPECT_TRUE(result.empty());
}

TEST_F(GeometricFilterByPolygonTest, IntersectionExactlyThreshold) {
  auto proto = MakeSquareProto(0, 0, 10, 10, PolygonType::INCLUSION, 1, 0.25f);
  std::vector<Polygon> polygons = {MakePolygon(proto, {0})};

  Detection det;
  det.class_id = 0;
  det.box = cv::Rect(
      5, 5, 10, 10);  // площадь пересечения 25, площадь бокса 100, 25/100=0.25

  auto result = filter_->Apply({det}, polygons);
  ASSERT_EQ(result.size(), 1);
}