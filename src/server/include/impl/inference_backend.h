#pragma once

#include <opencv2/dnn.hpp>

#include "interfaces/i_inference_backend.h"

/**
 * @class OpenCvDnnBackend
 * @brief Бэкенд инференса, использующий модуль DNN библиотеки OpenCV.
 */
class OpenCvDnnBackend : public IInferenceBackend {
 public:
  /**
   * @brief Загружает модель YOLO формата Darknet.
   * @param config Путь к .cfg файлу.
   * @param weights Путь к .weights файлу.
   * @throws std::runtime_error если сеть не загружена.
   */
  void loadModel(const std::string& config,
                 const std::string& weights) override;

  /**
   * @brief Прямой проход.
   * @param blob Входной blob.
   * @return std::vector<cv::Mat> Выходные слои.
   */
  std::vector<cv::Mat> forward(const cv::Mat& blob) override;

 private:
  cv::dnn::Net net_;  ///< Загруженная сеть OpenCV DNN.
};