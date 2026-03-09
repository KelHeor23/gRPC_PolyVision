#pragma once
/**
 * @file yolo_detector.h
 * @brief Реализация детектора на базе YOLO (Darknet).
 */
#include "config.h"
#include "interfaces/i_detector.h"

struct Detection;

/**
 * @class YoloDetector
 * @brief Выполняет детекцию объектов с помощью нейросети YOLO.
 */
class YoloDetector : public IDetector {
 public:
  /**
   * @brief Конструктор, загружающий сеть и настраивающий разрешённые классы.
   * @param cfg Конфигурация.
   * @throws std::runtime_error если сеть не загружена.
   */
  YoloDetector(const Config& cfg);

  /**
   * @brief Детектирует объекты на изображении.
   * @param image Входное изображение.
   * @return Вектор обнаруженных объектов.
   */
  std::vector<Detection> Detect(const cv::Mat& image) override;

 private:
  /**
   * @brief Обрабатывает выходные тензоры сети, применяет NMS и фильтрацию по
   * классам.
   * @param outs Выходные слои сети.
   * @param img_size Размер исходного изображения.
   * @return Вектор детекций.
   */
  std::vector<Detection> ProcessOutput(const std::vector<cv::Mat>& outs,
                                       const cv::Size& img_size) const;

 private:
  cv::dnn::Net net_;      ///< Загруженная сеть YOLO
  cv::Size input_size_;   ///< Размер входа сети
  float conf_threshold_;  ///< Порог уверенности
  float nms_threshold_;   ///< Порог NMS
};
