#pragma once
#include <opencv2/core.hpp>
#include <optional>
#include <vector>

class IImageEncoder {
 public:
  virtual ~IImageEncoder() = default;
  virtual std::optional<std::vector<uint8_t>> encode(const cv::Mat& image,
                                                     const std::string& format,
                                                     int quality) = 0;
  virtual std::optional<cv::Mat> decode(const std::vector<uint8_t>& data,
                                        int flags) = 0;
};