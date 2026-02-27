#pragma once
/**
 * @file IDetector.h
 * @brief Интерфейс детектора объектов.
 */
#include <opencv2/opencv.hpp>
#include <vector>

struct Detection;

/**
 * @interface IDetector
 * @brief Обнаруживает объекты на изображении.
 */
class IDetector {
 public:
  virtual ~IDetector() = default;
  /**
   * @brief Выполняет детекцию.
   * @param image Входное изображение.
   * @return Вектор обнаруженных объектов.
   */
  virtual std::vector<Detection> detect(const cv::Mat& image) = 0;
};