#pragma once

/**
 * @file OpencvDisplay.h
 * @brief Реализация интерфейса отображения с помощью OpenCV.
 */

#include <opencv2/core.hpp>
#include <string>

#include "interfaces/IDisplay.h"

/**
 * @class OpenCVDisplay
 * @brief Отображает изображения в окнах OpenCV и обрабатывает клавиши.
 */
class OpenCVDisplay : public IDisplay {
 public:
  /**
   * @brief Показывает изображение в окне.
   * @param image Изображение (cv::Mat).
   * @param windowName Имя окна.
   */
  void show(const cv::Mat& image, const std::string& windowName) override;

  /**
   * @brief Ожидает нажатие клавиши в течение заданного времени.
   * @param delay Время ожидания в миллисекундах (0 – бесконечно).
   */
  void waitKey(int delay) override;
};