#include <gtest/gtest.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "impl/OpenCVEncoder.h"

class OpenCVEncoderTest : public ::testing::Test {
 protected:
  OpenCVEncoder encoder;
  cv::Mat testImage;

  void SetUp() override {
    testImage = cv::Mat(100, 100, CV_8UC3, cv::Scalar(100, 150, 200));
  }
};

TEST_F(OpenCVEncoderTest, EncodeAndDecodePng) {
  auto encoded = encoder.encode(testImage, ".png", 0);
  ASSERT_TRUE(encoded.has_value());
  EXPECT_FALSE(encoded->empty());

  auto decoded = encoder.decode(*encoded, cv::IMREAD_COLOR);
  ASSERT_TRUE(decoded.has_value());
  EXPECT_EQ(decoded->rows, testImage.rows);
  EXPECT_EQ(decoded->cols, testImage.cols);
  EXPECT_EQ(decoded->type(), testImage.type());
  double diff = cv::norm(testImage, *decoded, cv::NORM_L1);
  EXPECT_EQ(diff, 0);
}

TEST_F(OpenCVEncoderTest, EncodeWithUnsupportedFormatReturnsNullopt) {
  auto encoded = encoder.encode(testImage, ".bmp", 0);
  auto result = encoder.encode(testImage, ".xyz", 0);
  EXPECT_FALSE(result.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeEmptyDataReturnsNullopt) {
  std::vector<uint8_t> empty;
  auto decoded = encoder.decode(empty, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeCorruptedDataReturnsNullopt) {
  std::vector<uint8_t> corrupted = {0, 1, 2, 3, 4, 5};
  auto decoded = encoder.decode(corrupted, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}