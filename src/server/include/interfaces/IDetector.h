#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

struct Detection;

class IDetector {
 public:
  virtual ~IDetector() = default;
  virtual std::vector<Detection> detect(const cv::Mat& image) = 0;
};