#pragma once
#include "Config.h"
#include "interfaces/IDetector.h"

struct Detection;
class IClassMapper;

class YoloDetector : public IDetector {
 public:
  YoloDetector(const Config& cfg, std::shared_ptr<IClassMapper> mapper);

  std::vector<Detection> detect(const cv::Mat& image) override;

 private:
  std::vector<Detection> processOutput(const std::vector<cv::Mat>& outs,
                                       const cv::Size& imgSize);

 private:
  cv::dnn::Net net_;
  cv::Size inputSize_;
  float confThreshold_;
  float nmsThreshold_;
  std::vector<int> allowedIds_;
};
