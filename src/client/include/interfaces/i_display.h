#pragma once
/**
 * @file i_display.h
 * @brief Интерфейс для отображения изображений.
 */

#include <opencv2/core.hpp>
#include <string>

/**
 * @interface IDisplay
 * @brief Абстрактный базовый класс для вывода изображений и обработки ввода.
 */
class IDisplay {
 public:
  virtual ~IDisplay() = default;
  /**
   * @brief Отобразить изображение.
   * @param image Изображение для отображения.
   * @param window_name Имя окна.
   */
  virtual void Show(const cv::Mat& image, const std::string& window_name) = 0;
  /**
   * @brief Ожидать нажатие клавиши.
   * @param delay Время ожидания (мс).
   */
  virtual void WaitKey(int delay) = 0;
};