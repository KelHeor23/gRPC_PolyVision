#pragma once

#include <opencv2/core.hpp>

#include "interfaces/i_image_preprocessor.h"

/**
 * @class YoloPreprocessor
 * @brief Предобработчик для моделей YOLO: создаёт blob с масштабированием до
 * входного размера.
 */
class YoloPreprocessor : public IImagePreprocessor {
 public:
  /**
   * @brief Конструктор.
   * @param inputSize Целевой размер входа сети (ширина, высота).
   */
  explicit YoloPreprocessor(cv::Size inputSize);

  /**
   * @brief Создаёт blob из изображения: масштабирование, нормализация, смена
   * каналов.
   * @param image Входное изображение.
   * @return cv::Mat Blob, готовый для YOLO.
   */
  cv::Mat preprocess(const cv::Mat& image) override;

 private:
  cv::Size inputSize_;  ///< Ожидаемый размер входа сети.
};
