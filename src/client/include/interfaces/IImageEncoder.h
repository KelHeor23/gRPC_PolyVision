#pragma once
#include <opencv2/core.hpp>
#include <vector>

class IImageEncoder {
 public:
  virtual ~IImageEncoder() = default;
  virtual std::vector<uint8_t> encode(const cv::Mat& image,
                                      const std::string& format,
                                      int quality) = 0;
  virtual cv::Mat decode(const std::vector<uint8_t>& data, int flags) = 0;
};