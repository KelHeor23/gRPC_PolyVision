#include "polygons/file_reader.h"

#include <gtest/gtest.h>

#include <filesystem>
using namespace testing;

class FileReaderTest : public Test {
 protected:
  void SetUp() override {
    temp_file_ = std::filesystem::temp_directory_path() / "test_file.txt";
    std::ofstream ofs(temp_file_);
    ofs << "line1\nline2\n";
  }

  void TearDown() override { std::filesystem::remove(temp_file_); }

  std::filesystem::path temp_file_;
};

TEST_F(FileReaderTest, ReadExistingFileReturnsContent) {
  FileReader reader;
  auto content = reader.Read(temp_file_.string());
  ASSERT_TRUE(content.has_value());
  EXPECT_EQ(*content, "line1\nline2\n");
}

TEST_F(FileReaderTest, ReadNonExistingFileReturnsNullopt) {
  FileReader reader;
  auto content = reader.Read("non_existing_file.txt");
  EXPECT_FALSE(content.has_value());
}