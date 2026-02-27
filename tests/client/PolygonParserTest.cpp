#include <gtest/gtest.h>

#include <boost/json.hpp>
#include <string>
#include <vector>

#include "polygons/PolygonParser.h"

using namespace testing;

class PolygonParserTest : public ::testing::Test {
 protected:
  PolygonParser parser;

  std::string makePolygonJson(int type, int priority, double threshold,
                              const std::vector<std::pair<int, int>>& points) {
    std::string pts = "[";
    for (size_t i = 0; i < points.size(); ++i) {
      if (i) pts += ",";
      pts += R"({"x":)" + std::to_string(points[i].first) + R"(,"y":)" +
             std::to_string(points[i].second) + "}";
    }
    pts += "]";
    return R"({"type":)" + std::to_string(type) + R"(,"priority":)" +
           std::to_string(priority) + R"(,"threshold":)" +
           std::to_string(threshold) + R"(,"points":)" + pts + "}";
  }

  // Создаёт полный JSON с массивом полигонов
  boost::json::value createJsonWithPolygons(
      const std::vector<std::string>& polyJsons) {
    std::string arr = "[";
    for (size_t i = 0; i < polyJsons.size(); ++i) {
      if (i) arr += ",";
      arr += polyJsons[i];
    }
    arr += "]";
    std::string full = R"({"polygons":)" + arr + "}";
    return boost::json::parse(full);
  }
};

TEST_F(PolygonParserTest, ParseValidJsonReturnsPolygons) {
  std::vector<std::string> polys = {
      makePolygonJson(0, 1, 0.5, {{0, 0}, {1, 0}, {1, 1}})};
  auto root = createJsonWithPolygons(polys);
  auto polygonsOpt = parser.parse(root);
  ASSERT_TRUE(polygonsOpt.has_value());
  const auto& polygons = *polygonsOpt;
  EXPECT_EQ(polygons.size(), 1);
  EXPECT_EQ(polygons[0].type(), 0);
  EXPECT_EQ(polygons[0].priority(), 1);
  EXPECT_FLOAT_EQ(polygons[0].threshold(), 0.5f);
  EXPECT_EQ(polygons[0].points_size(), 3);
}

TEST_F(PolygonParserTest, ParseJsonWithoutPolygonsReturnsNullopt) {
  std::string json = R"({"not_polygons": []})";
  auto value = boost::json::parse(json);
  auto polygonsOpt = parser.parse(value);
  EXPECT_FALSE(polygonsOpt.has_value());
}

TEST_F(PolygonParserTest, ParseEmptyJsonReturnsNullopt) {
  std::string empty = "";
  EXPECT_THROW(boost::json::parse(empty), boost::system::system_error);
}

TEST_F(PolygonParserTest, PolygonsNotArrayReturnsNullopt) {
  auto root = boost::json::parse(R"({"polygons": "not array"})");
  auto result = parser.parse(root);
  EXPECT_FALSE(result.has_value());
}

TEST_F(PolygonParserTest, ValidPolygonsReturned) {
  std::vector<std::string> polys = {
      makePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}, {10, 20}}),
      makePolygonJson(1, 2, 0.8, {{30, 30}, {40, 30}, {40, 40}})};
  auto root = createJsonWithPolygons(polys);
  auto result = parser.parse(root);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2);

  const auto& p0 = (*result)[0];
  EXPECT_EQ(p0.type(), 0);
  EXPECT_EQ(p0.priority(), 1);
  EXPECT_FLOAT_EQ(p0.threshold(), 0.5f);
  ASSERT_EQ(p0.points_size(), 4);
  EXPECT_EQ(p0.points(0).x(), 10);
  EXPECT_EQ(p0.points(0).y(), 10);
  EXPECT_EQ(p0.points(1).x(), 20);
  EXPECT_EQ(p0.points(1).y(), 10);
  EXPECT_EQ(p0.points(2).x(), 20);
  EXPECT_EQ(p0.points(2).y(), 20);
  EXPECT_EQ(p0.points(3).x(), 10);
  EXPECT_EQ(p0.points(3).y(), 20);

  const auto& p1 = (*result)[1];
  EXPECT_EQ(p1.type(), 1);
  EXPECT_EQ(p1.priority(), 2);
  EXPECT_FLOAT_EQ(p1.threshold(), 0.8f);
  ASSERT_EQ(p1.points_size(), 3);
}

TEST_F(PolygonParserTest, InvalidPolygonsAreSkipped) {
  std::vector<std::string> polys = {
      makePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}}),  // +
      makePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}}),            // -
      makePolygonJson(2, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}}),  // -
      makePolygonJson(0, 1, 1.5, {{10, 10}, {20, 10}, {20, 20}}),  // -
      makePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}})   // +
  };
  auto root = createJsonWithPolygons(polys);
  auto result = parser.parse(root);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->size(), 2);
}

TEST_F(PolygonParserTest, AllPolygonsInvalidReturnsNullopt) {
  std::vector<std::string> polys = {
      R"({"type":0,"priority":1,"threshold":0.5})",       // -
      makePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}})};  // -
  auto root = createJsonWithPolygons(polys);
  auto result = parser.parse(root);
  EXPECT_FALSE(result.has_value());
}

TEST_F(PolygonParserTest, InvalidPointCoordinatesRejectPolygon) {
  std::string poly =
      R"({"type":0,"priority":1,"threshold":0.5,"points":[{"x":10,"y":10},{"x":"a","y":20},{"x":30,"y":30}]})";
  auto root = createJsonWithPolygons({poly});
  auto result = parser.parse(root);
  EXPECT_FALSE(result.has_value());
}