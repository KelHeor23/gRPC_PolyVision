#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

#include "interfaces/IImageEncoder.h"

class OpenCVEncoder : public IImageEncoder {
 public:
  std::optional<std::vector<uint8_t>> encode(const cv::Mat& image,
                                             const std::string& format,
                                             int quality) override;
  std::optional<cv::Mat> decode(const std::vector<uint8_t>& data,
                                int flags) override;
};