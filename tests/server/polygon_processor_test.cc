#include "impl/polygon_processor.h"

#include <gtest/gtest.h>

#include "proto/ImageAnalysis.grpc.pb.h"

TEST(PolygonProcessorTest, SortsByPriorityAndType) {
  PolygonProcessor processor;
  std::vector<ImageDetection::Polygon> polygons;

  auto add_poly = [&](int priority, ImageDetection::PolygonType type) {
    ImageDetection::Polygon p;
    p.set_priority(priority);
    p.set_type(type);
    polygons.push_back(p);
  };

  add_poly(10, ImageDetection::PolygonType::INCLUSION);
  add_poly(5, ImageDetection::PolygonType::EXCLUSION);
  add_poly(10, ImageDetection::PolygonType::EXCLUSION);
  add_poly(1, ImageDetection::PolygonType::INCLUSION);

  processor.ProcessPolygons(polygons);

  ASSERT_EQ(polygons.size(), 4);
  EXPECT_EQ(polygons[0].priority(), 10);
  EXPECT_EQ(polygons[0].type(), ImageDetection::PolygonType::EXCLUSION);
  EXPECT_EQ(polygons[1].priority(), 10);
  EXPECT_EQ(polygons[1].type(), ImageDetection::PolygonType::INCLUSION);
  EXPECT_EQ(polygons[2].priority(), 5);
  EXPECT_EQ(polygons[2].type(), ImageDetection::PolygonType::EXCLUSION);
  EXPECT_EQ(polygons[3].priority(), 1);
  EXPECT_EQ(polygons[3].type(), ImageDetection::PolygonType::INCLUSION);
}