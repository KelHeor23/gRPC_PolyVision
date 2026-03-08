#pragma once

/**
 * @file opencv_display.h
 * @brief Реализация интерфейса отображения с помощью OpenCV.
 */

#include <opencv2/core.hpp>
#include <string>

#include "interfaces/i_display.h"

/**
 * @class OpenCVDisplay
 * @brief Отображает изображения в окнах OpenCV и обрабатывает клавиши.
 */
class OpenCVDisplay : public IDisplay {
 public:
  /**
   * @brief Показывает изображение в окне.
   * @param image Изображение (cv::Mat).
   * @param window_name Имя окна.
   */
  void Show(const cv::Mat& image, const std::string& window_name) override;

  /**
   * @brief Ожидает нажатие клавиши в течение заданного времени.
   * @param delay Время ожидания в миллисекундах (0 – бесконечно).
   */
  void WaitKey(int delay) override;
};