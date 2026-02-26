#include <gtest/gtest.h>

#include "impl/PolygonProcessor.h"
#include "proto/ImageAnalysis.grpc.pb.h"

TEST(PolygonProcessorTest, SortsByPriorityAndType) {
  PolygonProcessor processor;
  std::vector<ImageDetection::Polygon> polygons;

  auto addPoly = [&](int priority, ImageDetection::PolygonType type) {
    ImageDetection::Polygon p;
    p.set_priority(priority);
    p.set_type(type);
    polygons.push_back(p);
  };

  addPoly(10, ImageDetection::PolygonType::INCLUSION);
  addPoly(5, ImageDetection::PolygonType::EXCLUSION);
  addPoly(10, ImageDetection::PolygonType::EXCLUSION);
  addPoly(1, ImageDetection::PolygonType::INCLUSION);

  processor.processPolygons(polygons);

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