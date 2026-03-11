#pragma once

/**
 * @file i_image_preprocessor.h
 * @brief Объявление интерфейса предобработки изображения перед подачей в
 * нейросеть.
 */

#include <opencv2/core.hpp>
#include <vector>

struct Detection;

/**
 * @interface IImagePreprocessor
 * @brief Интерфейс предобработки изображения перед подачей в нейросеть.
 */
class IImagePreprocessor {
 public:
  virtual ~IImagePreprocessor() = default;

  /**
   * @brief Преобразует входное изображение в blob, пригодный для сети.
   * @param image Входное изображение (формат BGR).
   * @return cv::Mat Blob для прямого прохода сети.
   */
  virtual cv::Mat preprocess(const cv::Mat& image) = 0;
};
