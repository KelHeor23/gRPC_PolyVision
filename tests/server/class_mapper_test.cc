#include <gtest/gtest.h>

#include <fstream>

#include "impl/class_mapper.h"

class ClassMapperTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::ofstream file("test_classes.names");
    file << "person\ncar\ndog\ncat\n";
    file.close();
  }
  void TearDown() override { std::remove("test_classes.names"); }
};

TEST_F(ClassMapperTest, LoadClasses) {
  ClassMapper mapper("test_classes.names");
  EXPECT_EQ(mapper.GetSize(), 4);
  EXPECT_EQ(mapper.GetClassName(0), "person");
  EXPECT_EQ(mapper.GetClassName(3), "cat");
  EXPECT_EQ(mapper.GetClassName(10), "unknown");
}

TEST_F(ClassMapperTest, GetAllowedIds) {
  ClassMapper mapper("test_classes.names");
  std::vector<std::string> allowed = {"dog", "cat", "unknown"};
  auto ids = mapper.GetAllowedIds(allowed);
  std::vector<int> expected = {2, 3};
  EXPECT_EQ(ids, expected);
}

TEST_F(ClassMapperTest, FileNotFound) {
  EXPECT_THROW(ClassMapper mapper("nonexistent.names"), std::runtime_error);
}