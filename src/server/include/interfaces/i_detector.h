#pragma once
/**
 * @file i_detector.h
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
  virtual std::vector<Detection> Detect(const cv::Mat& image) = 0;
};