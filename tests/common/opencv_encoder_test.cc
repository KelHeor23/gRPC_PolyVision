#include "impl/opencv_encoder.h"

#include <gtest/gtest.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

class OpenCVEncoderTest : public ::testing::Test {
 protected:
  OpenCVEncoder encoder;
  cv::Mat test_image;

  void SetUp() override {
    test_image = cv::Mat(100, 100, CV_8UC3, cv::Scalar(100, 150, 200));
  }
};

TEST_F(OpenCVEncoderTest, EncodeAndDecodePng) {
  auto encoded = encoder.Encode(test_image, ".png", 0);
  ASSERT_TRUE(encoded.has_value());
  EXPECT_FALSE(encoded->empty());

  auto decoded = encoder.Decode(*encoded, cv::IMREAD_COLOR);
  ASSERT_TRUE(decoded.has_value());
  EXPECT_EQ(decoded->rows, test_image.rows);
  EXPECT_EQ(decoded->cols, test_image.cols);
  EXPECT_EQ(decoded->type(), test_image.type());
  double diff = cv::norm(test_image, *decoded, cv::NORM_L1);
  EXPECT_EQ(diff, 0);
}

TEST_F(OpenCVEncoderTest, EncodeWithUnsupportedFormatReturnsNullopt) {
  auto encoded = encoder.Encode(test_image, ".bmp", 0);
  auto result = encoder.Encode(test_image, ".xyz", 0);
  EXPECT_FALSE(result.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeEmptyDataReturnsNullopt) {
  std::vector<uint8_t> empty;
  auto decoded = encoder.Decode(empty, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeCorruptedDataReturnsNullopt) {
  std::vector<uint8_t> corrupted = {0, 1, 2, 3, 4, 5};
  auto decoded = encoder.Decode(corrupted, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}