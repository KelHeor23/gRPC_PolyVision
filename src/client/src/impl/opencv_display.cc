#include "impl/opencv_display.h"

#include <opencv2/highgui.hpp>

void OpenCVDisplay::Show(const cv::Mat& image, const std::string& windowName) {
  cv::imshow(windowName, image);
}
void OpenCVDisplay::WaitKey(int delay) { cv::waitKey(delay); }