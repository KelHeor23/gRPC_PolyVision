#pragma once
#include <opencv2/core.hpp>
#include <string>
class IDisplay {
 public:
  virtual ~IDisplay() = default;
  virtual void show(const cv::Mat& image, const std::string& windowName) = 0;
  virtual void waitKey(int delay) = 0;
};