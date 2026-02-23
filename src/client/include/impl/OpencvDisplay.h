#pragma once
#include <opencv2/core.hpp>
#include <string>

#include "interfaces/IDisplay.h"

class OpenCVDisplay : public IDisplay {
 public:
  void show(const cv::Mat& image, const std::string& windowName) override;
  void waitKey(int delay) override;
};