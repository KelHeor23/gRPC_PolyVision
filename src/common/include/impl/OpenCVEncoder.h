#pragma once
/**
 * @file OpenCVEncoder.h
 * @brief Реализация интерфейса IImageEncoder с использованием OpenCV.
 */
#include <opencv2/core.hpp>
#include <string>
#include <vector>

#include "interfaces/IImageEncoder.h"

/**
 * @class OpenCVEncoder
 * @brief Кодирует изображения в буфер и декодирует обратно с помощью OpenCV.
 *
 * Поддерживает форматы, определяемые OpenCV (".jpg").
 */

class OpenCVEncoder : public IImageEncoder {
 public:
  /**
   * @brief Кодирует изображение в формат, заданный параметром format.
   * @param image Исходное изображение (cv::Mat).
   * @param format Расширение формата (например, ".jpg").
   * @param quality Качество сжатия (для JPEG от 0 до 100).
   * @return Вектор байт с закодированным изображением или std::nullopt при
   * ошибке.
   */
  std::optional<std::vector<uint8_t>> encode(const cv::Mat& image,
                                             const std::string& format,
                                             int quality) override;
  /**
   * @brief Декодирует буфер данных в изображение cv::Mat.
   * @param data Вектор байт с закодированным изображением.
   * @param flags Флаги imdecode (например, cv::IMREAD_COLOR).
   * @return Декодированное изображение или std::nullopt при ошибке.
   */
  std::optional<cv::Mat> decode(const std::vector<uint8_t>& data,
                                int flags) override;
};