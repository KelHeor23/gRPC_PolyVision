#pragma once
#include <opencv2/opencv.hpp>

struct Detection {
  int classId;
  float confidence;
  cv::Rect box;
};