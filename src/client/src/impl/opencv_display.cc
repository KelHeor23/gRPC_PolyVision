#include "impl/opencv_display.h"

#include <opencv2/highgui.hpp>

void OpenCVDisplay::Show(const cv::Mat& image, const std::string& window_name) {
  cv::imshow(window_name, image);
}
void OpenCVDisplay::WaitKey(int delay) { cv::waitKey(delay); }