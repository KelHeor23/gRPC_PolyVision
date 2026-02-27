#pragma once
/**
 * @file IImageEncoder.h
 * @brief Интерфейс для кодирования/декодирования изображений.
 */
#include <opencv2/core.hpp>
#include <optional>
#include <vector>

/**
 * @interface IImageEncoder
 * @brief Абстрактный базовый класс, определяющий методы преобразования
 *        изображения в байтовый буфер и обратно.
 */
class IImageEncoder {
 public:
  virtual ~IImageEncoder() = default;
  /**
   * @brief Кодирует изображение в формат, заданный параметром format.
   * @param image Исходное изображение (cv::Mat).
   * @param format Расширение формата (например, ".jpg").
   * @param quality Качество сжатия (для JPEG от 0 до 100).
   * @return Вектор байт с закодированным изображением или std::nullopt при
   * ошибке.
   */
  virtual std::optional<std::vector<uint8_t>> encode(const cv::Mat& image,
                                                     const std::string& format,
                                                     int quality) = 0;

  /**
   * @brief Декодирует буфер данных в изображение cv::Mat.
   * @param data Вектор байт с закодированным изображением.
   * @param flags Флаги imdecode (например, cv::IMREAD_COLOR).
   * @return Декодированное изображение или std::nullopt при ошибке.
   */
  virtual std::optional<cv::Mat> decode(const std::vector<uint8_t>& data,
                                        int flags) = 0;
};