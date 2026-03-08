#include "impl/opencv_encoder.h"

#include <gtest/gtest.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

class OpenCVEncoderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_image_ = cv::Mat(100, 100, CV_8UC3, cv::Scalar(100, 150, 200));
  }

  OpenCVEncoder encoder_;
  cv::Mat test_image_;
};

TEST_F(OpenCVEncoderTest, EncodeAndDecodePng) {
  auto encoded = encoder_.Encode(test_image_, ".png", 0);
  ASSERT_TRUE(encoded.has_value());
  EXPECT_FALSE(encoded->empty());

  auto decoded = encoder_.Decode(*encoded, cv::IMREAD_COLOR);
  ASSERT_TRUE(decoded.has_value());
  EXPECT_EQ(decoded->rows, test_image_.rows);
  EXPECT_EQ(decoded->cols, test_image_.cols);
  EXPECT_EQ(decoded->type(), test_image_.type());
  double diff = cv::norm(test_image_, *decoded, cv::NORM_L1);
  EXPECT_EQ(diff, 0);
}

TEST_F(OpenCVEncoderTest, EncodeWithUnsupportedFormatReturnsNullopt) {
  auto encoded = encoder_.Encode(test_image_, ".bmp", 0);
  auto result = encoder_.Encode(test_image_, ".xyz", 0);
  EXPECT_FALSE(result.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeEmptyDataReturnsNullopt) {
  std::vector<uint8_t> empty;
  auto decoded = encoder_.Decode(empty, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}

TEST_F(OpenCVEncoderTest, DecodeCorruptedDataReturnsNullopt) {
  std::vector<uint8_t> corrupted = {0, 1, 2, 3, 4, 5};
  auto decoded = encoder_.Decode(corrupted, cv::IMREAD_COLOR);
  EXPECT_FALSE(decoded.has_value());
}