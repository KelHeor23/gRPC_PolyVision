#include "polygons/polygon_parser.h"

#include <gtest/gtest.h>

#include <boost/json.hpp>
#include <string>
#include <vector>

using namespace testing;

class PolygonParserTest : public ::testing::Test {
 protected:
  std::string MakePolygonJson(int type, int priority, double threshold,
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

  // Создаёт полный JSON только с массивом полигонов (без class_names)
  boost::json::value CreateJsonWithPolygons(
      const std::vector<std::string>& poly_jsons) {
    std::string arr = "[";
    for (size_t i = 0; i < poly_jsons.size(); ++i) {
      if (i) arr += ",";
      arr += poly_jsons[i];
    }
    arr += "]";
    std::string full = R"({"polygons":)" + arr + "}";
    return boost::json::parse(full);
  }

  // Создаёт полный JSON с полигонами и опциональными class_names
  boost::json::value CreateFullJson(
      const std::vector<std::string>& poly_jsons,
      const std::vector<std::string>& class_names = {}) {
    std::string arr = "[";
    for (size_t i = 0; i < poly_jsons.size(); ++i) {
      if (i) arr += ",";
      arr += poly_jsons[i];
    }
    arr += "]";

    std::string full = R"({"polygons":)" + arr;
    if (!class_names.empty()) {
      full += R"(,"class_names":[)";
      for (size_t i = 0; i < class_names.size(); ++i) {
        if (i) full += ",";
        full += R"(")" + class_names[i] + R"(")";
      }
      full += "]";
    }
    full += "}";
    return boost::json::parse(full);
  }

  PolygonParser parser_;
};

TEST_F(PolygonParserTest, ParseValidJsonReturnsPolygonList) {
  std::vector<std::string> polys = {
      MakePolygonJson(0, 1, 0.5, {{0, 0}, {1, 0}, {1, 1}})};
  auto root = CreateFullJson(polys);  // без class_names
  auto result_opt = parser_.Parse(root);
  ASSERT_TRUE(result_opt.has_value());
  const auto& polygon_list = *result_opt;
  EXPECT_EQ(polygon_list.polygons_size(), 1);
  EXPECT_EQ(polygon_list.class_names_size(), 0);  // нет class_names

  const auto& p = polygon_list.polygons(0);
  EXPECT_EQ(p.type(), 0);
  EXPECT_EQ(p.priority(), 1);
  EXPECT_FLOAT_EQ(p.threshold(), 0.5f);
  EXPECT_EQ(p.points_size(), 3);
}

TEST_F(PolygonParserTest, ParseJsonWithClassNamesReturnsBoth) {
  std::vector<std::string> polys = {
      MakePolygonJson(0, 1, 0.5, {{0, 0}, {1, 0}, {1, 1}})};
  std::vector<std::string> class_names = {"car", "pedestrian"};
  auto root = CreateFullJson(polys, class_names);
  auto result_opt = parser_.Parse(root);
  ASSERT_TRUE(result_opt.has_value());
  const auto& polygon_list = *result_opt;

  EXPECT_EQ(polygon_list.polygons_size(), 1);
  EXPECT_EQ(polygon_list.class_names_size(), 2);
  EXPECT_EQ(polygon_list.class_names(0), "car");
  EXPECT_EQ(polygon_list.class_names(1), "pedestrian");
}

TEST_F(PolygonParserTest, InvalidClassNamesFormatReturnsNullopt) {
  std::string json = R"({"polygons":[], "class_names": "not an array"})";
  auto root = boost::json::parse(json);
  auto result = parser_.Parse(root);
  EXPECT_FALSE(result.has_value());

  json = R"({"polygons":[], "class_names": [123, "car"]})";
  root = boost::json::parse(json);
  result = parser_.Parse(root);
  EXPECT_FALSE(result.has_value());
}

TEST_F(PolygonParserTest, ParseJsonWithoutPolygonsReturnsNullopt) {
  std::string json = R"({"not_polygons": []})";
  auto value = boost::json::parse(json);
  auto result_opt = parser_.Parse(value);
  EXPECT_FALSE(result_opt.has_value());
}

TEST_F(PolygonParserTest, ParseEmptyJsonThrows) {
  std::string empty = "";
  EXPECT_THROW(boost::json::parse(empty), boost::system::system_error);
}

TEST_F(PolygonParserTest, PolygonsNotArrayReturnsNullopt) {
  auto root = boost::json::parse(R"({"polygons": "not array"})");
  auto result = parser_.Parse(root);
  EXPECT_FALSE(result.has_value());
}

TEST_F(PolygonParserTest, ValidPolygonsReturned) {
  std::vector<std::string> polys = {
      MakePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}, {10, 20}}),
      MakePolygonJson(1, 2, 0.8, {{30, 30}, {40, 30}, {40, 40}})};
  auto root = CreateFullJson(polys);
  auto result_opt = parser_.Parse(root);
  ASSERT_TRUE(result_opt.has_value());
  const auto& polygon_list = *result_opt;
  ASSERT_EQ(polygon_list.polygons_size(), 2);

  const auto& p0 = polygon_list.polygons(0);
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

  const auto& p1 = polygon_list.polygons(1);
  EXPECT_EQ(p1.type(), 1);
  EXPECT_EQ(p1.priority(), 2);
  EXPECT_FLOAT_EQ(p1.threshold(), 0.8f);
  ASSERT_EQ(p1.points_size(), 3);
}

TEST_F(PolygonParserTest, InvalidPolygonsAreSkipped) {
  std::vector<std::string> polys = {
      MakePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}}),  // valid
      MakePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}}),  // invalid (2 points)
      MakePolygonJson(2, 1, 0.5,
                      {{10, 10}, {20, 10}, {20, 20}}),  // invalid type
      MakePolygonJson(0, 1, 1.5,
                      {{10, 10}, {20, 10}, {20, 20}}),  // invalid threshold
      MakePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}, {20, 20}})  // valid
  };
  auto root = CreateFullJson(polys);
  auto result_opt = parser_.Parse(root);
  ASSERT_TRUE(result_opt.has_value());
  EXPECT_EQ(result_opt->polygons_size(), 2);
}

TEST_F(PolygonParserTest, AllPolygonsInvalidReturnsNullopt) {
  std::vector<std::string> polys = {
      R"({"type":0,"priority":1,"threshold":0.5})",       // missing points
      MakePolygonJson(0, 1, 0.5, {{10, 10}, {20, 10}})};  // too few points
  auto root = CreateFullJson(polys);
  auto result_opt = parser_.Parse(root);
  EXPECT_FALSE(result_opt.has_value());
}

TEST_F(PolygonParserTest, InvalidPointCoordinatesRejectPolygon) {
  std::string poly =
      R"({"type":0,"priority":1,"threshold":0.5,"points":[{"x":10,"y":10},{"x":"a","y":20},{"x":30,"y":30}]})";
  auto root = CreateFullJson({poly});
  auto result_opt = parser_.Parse(root);
  EXPECT_FALSE(result_opt.has_value());
}