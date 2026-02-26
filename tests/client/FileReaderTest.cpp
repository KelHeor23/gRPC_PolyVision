#include <gtest/gtest.h>

#include <filesystem>

#include "polygons/FileReader.h"
using namespace testing;

class FileReaderTest : public Test {
 protected:
  void SetUp() override {
    tempFile = std::filesystem::temp_directory_path() / "test_file.txt";
    std::ofstream ofs(tempFile);
    ofs << "line1\nline2\n";
  }

  void TearDown() override { std::filesystem::remove(tempFile); }

  std::filesystem::path tempFile;
};

TEST_F(FileReaderTest, ReadExistingFileReturnsContent) {
  FileReader reader;
  auto content = reader.read(tempFile.string());
  ASSERT_TRUE(content.has_value());
  EXPECT_EQ(*content, "line1\nline2\n");
}

TEST_F(FileReaderTest, ReadNonExistingFileReturnsNullopt) {
  FileReader reader;
  auto content = reader.read("non_existing_file.txt");
  EXPECT_FALSE(content.has_value());
}