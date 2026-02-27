#pragma once
/**
 * @file IDisplay.h
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
   * @param windowName Имя окна.
   */
  virtual void show(const cv::Mat& image, const std::string& windowName) = 0;
  /**
   * @brief Ожидать нажатие клавиши.
   * @param delay Время ожидания (мс).
   */
  virtual void waitKey(int delay) = 0;
};