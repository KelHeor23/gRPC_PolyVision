#include "impl/YoloDetector.h"

#include <algorithm>
#include <opencv2/dnn.hpp>
#include <stdexcept>

#include "impl/ClassMapper.h"
#include "impl/Detection.h"

YoloDetector::YoloDetector(const Config& cfg,
                           std::shared_ptr<IClassMapper> mapper)
    : inputSize_(cfg.inputSize),
      confThreshold_(cfg.confThreshold),
      nmsThreshold_(cfg.nmsThreshold) {
  net_ = cv::dnn::readNetFromDarknet(cfg.modelConfig, cfg.modelWeights);
  if (net_.empty()) throw std::runtime_error("Failed to load YOLO network");

  allowedIds_ = mapper->getAllowedIds(cfg.allowedClasses);
}

std::vector<Detection> YoloDetector::detect(const cv::Mat& image) {
  std::cout << "Running YOLO detection on image of size " << image.size()
            << std::endl;
  cv::Mat blob;
  cv::dnn::blobFromImage(image, blob, 1 / 255.0, inputSize_, cv::Scalar(), true,
                         false);
  net_.setInput(blob);

  std::vector<cv::Mat> outs;
  std::vector<std::string> outNames = net_.getUnconnectedOutLayersNames();
  net_.forward(outs, outNames);

  return processOutput(outs, image.size());
}

std::vector<Detection> YoloDetector::processOutput(
    const std::vector<cv::Mat>& outs, const cv::Size& imgSize) {
  std::vector<cv::Rect> boxes;
  std::vector<float> confidences;
  std::vector<int> classIds;

  for (const auto& out : outs) {
    for (int i = 0; i < out.rows; ++i) {
      const float* data = out.ptr<float>(i);
      float objectness = data[4];
      if (objectness < confThreshold_) continue;

      cv::Mat scores(1, out.cols - 5, CV_32F, const_cast<float*>(data + 5));
      double confidence;
      cv::Point classIdPoint;
      cv::minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);
      confidence *= objectness;

      if (confidence >= confThreshold_) {
        int classId = classIdPoint.x;
        // Фильтр по разрешённым классам
        if (std::find(allowedIds_.begin(), allowedIds_.end(), classId) ==
            allowedIds_.end())
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
        classIds.push_back(classId);
      }
    }
  }

  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, confThreshold_, nmsThreshold_, indices);

  std::vector<Detection> detections;
  detections.reserve(indices.size());
  for (int idx : indices) {
    detections.push_back({classIds[idx], confidences[idx], boxes[idx]});
  }
  return detections;
}