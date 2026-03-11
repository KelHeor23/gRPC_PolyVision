#pragma once

/**
 * @file i_detection_output_parser.h
 * @brief Объявление интерфейса парсинга сырых выходов сети
 */

#include <opencv2/core.hpp>
#include <vector>

class Detection;

/**
 * @interface IDetectionOutputParser
 * @brief Интерфейс парсинга сырых выходов сети в объекты Detection.
 */
class IDetectionOutputParser {
 public:
  virtual ~IDetectionOutputParser() = default;

  /**
   * @brief Парсит выходы сети, применяет пороги и NMS.
   * @param outputs Вектор выходных тензоров сети.
   * @param imageSize Размер исходного изображения.
   * @param confThreshold Порог уверенности.
   * @param nmsThreshold Порог подавления немаксимумов.
   * @return std::vector<Detection> Итоговые детекции.
   */
  virtual std::vector<Detection> parse(const std::vector<cv::Mat>& outputs,
                                       const cv::Size& imageSize,
                                       float confThreshold,
                                       float nmsThreshold) = 0;
};