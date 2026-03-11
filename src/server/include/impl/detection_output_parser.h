#pragma once

#include "interfaces/i_detection_output_parser.h"

/**
 * @class YoloOutputParser
 * @brief Парсит выходной формат YOLOv4
 */
class YoloOutputParser : public IDetectionOutputParser {
 public:
  /**
   * @brief Парсит выходы, применяет порог уверенности и NMS.
   * @param outputs Выходные тензоры сети.
   * @param imageSize Размер исходного изображения.
   * @param confThreshold Порог уверенности.
   * @param nmsThreshold Порог перекрытия для NMS.
   * @return std::vector<Detection> Итоговые детекции.
   */
  std::vector<Detection> parse(const std::vector<cv::Mat>& outputs,
                               const cv::Size& imageSize, float confThreshold,
                               float nmsThreshold) override;
};