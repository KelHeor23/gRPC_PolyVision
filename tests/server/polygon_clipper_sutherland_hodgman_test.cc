#include "impl/polygon_clipper_sutherland_hodgman.h"

#include <gtest/gtest.h>

#include <opencv2/core.hpp>

#include "proto/ImageAnalysis.pb.h"

using ImageDetection::Point;

// Вспомогательная функция для создания точки
Point MakePoint(int x, int y) {
  Point p;
  p.set_x(x);
  p.set_y(y);
  return p;
}

// Проверка равенства двух векторов точек (порядок важен)
bool PointsEqual(const std::vector<Point>& a, const std::vector<Point>& b) {
  if (a.size() != b.size()) return false;
  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i].x() != b[i].x() || a[i].y() != b[i].y()) return false;
  }
  return true;
}

// Проверка, что множество точек совпадает (порядок не важен)
bool PointsSetEqual(const std::vector<Point>& a, const std::vector<Point>& b) {
  if (a.size() != b.size()) return false;
  for (const auto& p : a) {
    bool found = false;
    for (const auto& q : b) {
      if (p.x() == q.x() && p.y() == q.y()) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }
  return true;
}

TEST(SutherlandHodgmanClipperTest, PolygonInsideBox) {
  SutherlandHodgmanClipper clipper;
  std::vector<Point> poly = {MakePoint(10, 10), MakePoint(20, 10),
                             MakePoint(20, 20), MakePoint(10, 20)};
  cv::Rect box(5, 5, 20, 20);  // охватывает полигон
  auto result = clipper.GetInternalPolygon(poly, box);
  EXPECT_TRUE(PointsEqual(poly, result));
}

TEST(SutherlandHodgmanClipperTest, PolygonOutsideBox) {
  SutherlandHodgmanClipper clipper;
  std::vector<Point> poly = {MakePoint(100, 100), MakePoint(200, 100),
                             MakePoint(200, 200), MakePoint(100, 200)};
  cv::Rect box(0, 0, 50, 50);
  auto result = clipper.GetInternalPolygon(poly, box);
  EXPECT_TRUE(result.empty());
}

TEST(SutherlandHodgmanClipperTest, PolygonIntersectsBox) {
  SutherlandHodgmanClipper clipper;
  std::vector<Point> poly = {MakePoint(0, 0), MakePoint(100, 0),
                             MakePoint(100, 100), MakePoint(0, 100)};
  cv::Rect box(25, 25, 50, 50);
  auto result = clipper.GetInternalPolygon(poly, box);

  std::vector<Point> expected = {MakePoint(25, 25), MakePoint(75, 25),
                                 MakePoint(75, 75), MakePoint(25, 75)};
  ASSERT_EQ(result.size(), 4);
  EXPECT_TRUE(PointsSetEqual(result, expected));
}

TEST(SutherlandHodgmanClipperTest, PolygonCornerIntersection) {
  SutherlandHodgmanClipper clipper;
  std::vector<Point> poly = {MakePoint(0, 0), MakePoint(50, 0),
                             MakePoint(50, 50), MakePoint(0, 50)};
  cv::Rect box(25, 25, 50, 50);  // от (25,25) до (75,75)
  auto result = clipper.GetInternalPolygon(poly, box);

  std::vector<Point> expected = {MakePoint(25, 25), MakePoint(50, 25),
                                 MakePoint(50, 50), MakePoint(25, 50)};
  ASSERT_EQ(result.size(), 4);
  EXPECT_TRUE(PointsSetEqual(result, expected));
}

TEST(SutherlandHodgmanClipperTest, PolygonDegenerate) {
  SutherlandHodgmanClipper clipper;
  // линия из двух точек – алгоритм не должен упасть
  std::vector<Point> line = {MakePoint(10, 10), MakePoint(20, 20)};
  cv::Rect box(0, 0, 100, 100);
  EXPECT_NO_THROW(clipper.GetInternalPolygon(line, box));

  // одна точка
  std::vector<Point> point = {MakePoint(10, 10)};
  EXPECT_NO_THROW(clipper.GetInternalPolygon(point, box));
}