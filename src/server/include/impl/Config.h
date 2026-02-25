#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct Config {
  std::string modelWeights;
  std::string modelConfig;
  std::string classesFile;
  float confThreshold = 0.2f;
  float nmsThreshold = 0.4f;
  cv::Size inputSize = {640, 640};
  std::vector<std::string> allowedClasses = {"orange", "banana", "apple",
                                             "book",   "cat",    "dog"};
};