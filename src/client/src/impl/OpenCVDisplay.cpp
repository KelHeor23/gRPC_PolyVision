#include "impl/OpenCVDisplay.h"

#include <opencv2/highgui.hpp>

void OpenCVDisplay::show(const cv::Mat& image, const std::string& windowName) {
  cv::imshow(windowName, image);
}
void OpenCVDisplay::waitKey(int delay) { cv::waitKey(delay); }