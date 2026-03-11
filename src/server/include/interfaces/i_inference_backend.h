#pragma once

/**
 * @file i_inference_backend.h
 * @brief Объявление интерфейса для бэкендов инференса нейросетей
 */

#include <opencv2/core.hpp>
#include <vector>

/**
 * @interface IInferenceBackend
 * @brief интерфейс для бэкендов инференса нейросетей.
 */
class IInferenceBackend {
 public:
  virtual ~IInferenceBackend() = default;

  /**
   * @brief Загружает модель из файлов конфигурации и весов.
   * @param config Путь к файлу конфигурации модели (например, .cfg).
   * @param weights Путь к файлу весов модели (например, .weights).
   * @throws std::runtime_error в случае ошибки загрузки.
   */
  virtual void loadModel(const std::string& config,
                         const std::string& weights) = 0;

  /**
   * @brief Выполняет прямой проход сети.
   * @param blob Предобработанный входной blob.
   * @return std::vector<cv::Mat> Выходные тензоры сети.
   */
  virtual std::vector<cv::Mat> forward(const cv::Mat& blob) = 0;
};