#include "impl/yolo_detector.h"

#include <algorithm>
#include <opencv2/dnn.hpp>
#include <stdexcept>

#include "impl/class_mapper.h"
#include "impl/detection.h"

YoloDetector::YoloDetector(const Config& cfg,
                           std::shared_ptr<IClassMapper> mapper)
    : input_size_(cfg.input_size),
      conf_threshold_(cfg.conf_threshold),
      nms_threshold_(cfg.nms_threshold) {
  net_ = cv::dnn::readNetFromDarknet(cfg.model_config, cfg.model_weights);
  if (net_.empty()) throw std::runtime_error("Failed to load YOLO network");

  allowed_ids_ = mapper->GetAllowedIds(cfg.allowed_classes);
}

std::vector<Detection> YoloDetector::Detect(const cv::Mat& image) {
  cv::Mat blob;
  cv::dnn::blobFromImage(image, blob, 1 / 255.0, input_size_, cv::Scalar(),
                         true, false);
  net_.setInput(blob);

  std::vector<cv::Mat> outs;
  std::vector<std::string> outNames = net_.getUnconnectedOutLayersNames();
  net_.forward(outs, outNames);

  return ProcessOutput(outs, image.size());
}

std::vector<Detection> YoloDetector::ProcessOutput(
    const std::vector<cv::Mat>& outs, const cv::Size& imgSize) {
  std::vector<cv::Rect> boxes;
  std::vector<float> confidences;
  std::vector<int> class_ids;

  for (const auto& out : outs) {
    for (int i = 0; i < out.rows; ++i) {
      const float* data = out.ptr<float>(i);
      float objectness = data[4];
      if (objectness < conf_threshold_) continue;

      cv::Mat scores(1, out.cols - 5, CV_32F, const_cast<float*>(data + 5));
      double confidence;
      cv::Point class_id_point;
      cv::minMaxLoc(scores, nullptr, &confidence, nullptr, &class_id_point);
      confidence *= objectness;

      if (confidence >= conf_threshold_) {
        int class_id = class_id_point.x;
        // Фильтр по разрешённым классам
        if (std::find(allowed_ids_.begin(), allowed_ids_.end(), class_id) ==
            allowed_ids_.end())
          continue;

        int centerX = static_cast<int>(data[0] * imgSize.width);
        int centerY = static_cast<int>(data[1] * imgSize.height);
        int width = static_cast<int>(data[2] * imgSize.width);
        int height = static_cast<int>(data[3] * imgSize.height);
        int left = centerX - width / 2;
        int top = centerY - height / 2;

        cv::Rect box(left, top, width, height);

        // Обрезаем бокс по границам изображения
        box &= cv::Rect(0, 0, imgSize.width, imgSize.height);
        if (box.area() <= 0) continue;

        boxes.push_back(box);
        confidences.push_back(static_cast<float>(confidence));
        class_ids.push_back(class_id);
      }
    }
  }

  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, conf_threshold_, nms_threshold_,
                    indices);

  std::vector<Detection> detections;
  detections.reserve(indices.size());
  for (int idx : indices) {
    detections.push_back({class_ids[idx], confidences[idx], boxes[idx]});
  }
  return detections;
}