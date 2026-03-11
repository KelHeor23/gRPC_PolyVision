#include "impl/inference_backend.h"

void OpenCvDnnBackend::loadModel(const std::string& config,
                                 const std::string& weights) {
  net_ = cv::dnn::readNetFromDarknet(config, weights);
  if (net_.empty())
    throw std::runtime_error("Не удалось загрузить сеть YOLO из " + config +
                             " и " + weights);
}

std::vector<cv::Mat> OpenCvDnnBackend::forward(const cv::Mat& blob) {
  net_.setInput(blob);
  std::vector<cv::Mat> outs;
  std::vector<std::string> outNames = net_.getUnconnectedOutLayersNames();
  net_.forward(outs, outNames);
  return outs;
}
