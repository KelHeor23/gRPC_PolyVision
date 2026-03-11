#include "impl/detection_output_parser.h"

#include "impl/detection.h"

std::vector<Detection> YoloOutputParser::parse(
    const std::vector<cv::Mat>& outputs, const cv::Size& imageSize,
    float confThreshold, float nmsThreshold) {
  std::vector<cv::Rect> boxes;
  std::vector<float> confidences;
  std::vector<int> class_ids;

  for (const auto& out : outputs) {
    for (int i = 0; i < out.rows; ++i) {
      const float* data = out.ptr<float>(i);
      float objectness = data[4];
      if (objectness < confThreshold) continue;

      cv::Mat scores(1, out.cols - 5, CV_32F, const_cast<float*>(data + 5));
      double confidence;
      cv::Point class_id_point;
      cv::minMaxLoc(scores, nullptr, &confidence, nullptr, &class_id_point);
      confidence *= objectness;

      if (confidence >= confThreshold) {
        int class_id = class_id_point.x;
        int center_x = static_cast<int>(data[0] * imageSize.width);
        int center_y = static_cast<int>(data[1] * imageSize.height);
        int width = static_cast<int>(data[2] * imageSize.width);
        int height = static_cast<int>(data[3] * imageSize.height);
        int left = center_x - width / 2;
        int top = center_y - height / 2;

        cv::Rect box(left, top, width, height);
        box &= cv::Rect(0, 0, imageSize.width, imageSize.height);
        if (box.area() <= 0) continue;

        boxes.push_back(box);
        confidences.push_back(static_cast<float>(confidence));
        class_ids.push_back(class_id);
      }
    }
  }

  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

  std::vector<Detection> detections;
  detections.reserve(indices.size());
  for (int idx : indices) {
    detections.push_back({class_ids[idx], confidences[idx], boxes[idx]});
  }
  return detections;
}