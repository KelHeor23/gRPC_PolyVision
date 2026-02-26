#include <gtest/gtest.h>

#include "polygons/JsonParser.h"

using namespace testing;

TEST(JsonParserTest, ParseValidJsonReturnsValue) {
  JsonParser parser;
  std::string json = R"({"key": "value"})";
  auto val = parser.parse(json);
  ASSERT_TRUE(val.has_value());
  EXPECT_EQ(val->as_object().at("key").as_string(), "value");
}

TEST(JsonParserTest, ParseInvalidJsonReturnsNullopt) {
  JsonParser parser;
  std::string invalid = R"({key: value})";
  auto val = parser.parse(invalid);
  EXPECT_FALSE(val.has_value());
}

TEST(JsonParserTest, ParseEmptyJsonReturnsNullopt) {
  JsonParser parser;
  std::string empty = "";
  auto val = parser.parse(empty);
  EXPECT_FALSE(val.has_value());
}