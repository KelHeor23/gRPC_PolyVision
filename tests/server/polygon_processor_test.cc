#include "impl/polygon_processor.h"

#include <gtest/gtest.h>

#include <memory>

#include "impl/class_mapper.h"
#include "impl/polygon.h"
#include "proto/ImageAnalysis.grpc.pb.h"

// Мок для IClassMapper
class MockMapper : public IClassMapper {
 public:
  std::string GetClassName(int class_id) const override { return ""; }
  int GetSize() const override { return 0; }
  std::vector<int> GetAllowedIds(
      const std::vector<std::string>& allowed_names) const override {
    std::vector<int> ids;
    for (const auto& name : allowed_names) {
      if (name == "person")
        ids.push_back(0);
      else if (name == "car")
        ids.push_back(1);
      else if (name == "dog")
        ids.push_back(2);
    }
    return ids;
  }
};

TEST(PolygonProcessorTest, SortsByPriorityAndType) {
  auto mapper = std::make_shared<MockMapper>();
  PolygonProcessor processor(mapper);
  std::vector<ImageDetection::Polygon> input;

  auto add_poly = [&](int priority, ImageDetection::PolygonType type) {
    ImageDetection::Polygon p;
    p.set_priority(priority);
    p.set_type(type);
    p.add_class_names("person");  // чтобы allowed_id не был пустым
    input.push_back(p);
  };

  add_poly(10, ImageDetection::PolygonType::INCLUSION);
  add_poly(5, ImageDetection::PolygonType::EXCLUSION);
  add_poly(10, ImageDetection::PolygonType::EXCLUSION);
  add_poly(1, ImageDetection::PolygonType::INCLUSION);

  auto result = processor.ProcessPolygons(std::move(input));

  ASSERT_EQ(result.size(), 4);
  // Сортировка по убыванию приоритета, затем по убыванию типа
  EXPECT_EQ(result[0].polygon.priority(), 10);
  EXPECT_EQ(result[0].polygon.type(), ImageDetection::PolygonType::EXCLUSION);
  EXPECT_EQ(result[1].polygon.priority(), 10);
  EXPECT_EQ(result[1].polygon.type(), ImageDetection::PolygonType::INCLUSION);
  EXPECT_EQ(result[2].polygon.priority(), 5);
  EXPECT_EQ(result[2].polygon.type(), ImageDetection::PolygonType::EXCLUSION);
  EXPECT_EQ(result[3].polygon.priority(), 1);
  EXPECT_EQ(result[3].polygon.type(), ImageDetection::PolygonType::INCLUSION);
}

TEST(PolygonProcessorTest, AllowedIdsFilledCorrectly) {
  auto mapper = std::make_shared<MockMapper>();
  PolygonProcessor processor(mapper);
  std::vector<ImageDetection::Polygon> input;

  ImageDetection::Polygon p;
  p.add_class_names("person");
  p.add_class_names("car");
  input.push_back(p);

  auto result = processor.ProcessPolygons(std::move(input));
  ASSERT_EQ(result.size(), 1);
  std::vector<int> expected = {0, 1};  // person -> 0, car -> 1
  EXPECT_EQ(result[0].allowed_id, expected);
}