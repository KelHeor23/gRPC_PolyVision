#include "impl/image_preprocessor.h"

#include <opencv2/dnn.hpp>
#include <stdexcept>

#include "impl/detection.h"

YoloPreprocessor::YoloPreprocessor(cv::Size inputSize)
    : inputSize_(inputSize) {}

cv::Mat YoloPreprocessor::preprocess(const cv::Mat& image) {
  cv::Mat blob;
  cv::dnn::blobFromImage(image, blob, 1 / 255.0, inputSize_, cv::Scalar(), true,
                         false);
  return blob;
}
