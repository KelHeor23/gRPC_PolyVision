#pragma once
/**
 * @file YoloDetector.h
 * @brief Реализация детектора на базе YOLO (Darknet).
 */
#include "Config.h"
#include "interfaces/IDetector.h"

struct Detection;
class IClassMapper;

/**
 * @class YoloDetector
 * @brief Выполняет детекцию объектов с помощью нейросети YOLO.
 */
class YoloDetector : public IDetector {
 public:
  /**
   * @brief Конструктор, загружающий сеть и настраивающий разрешённые классы.
   * @param cfg Конфигурация.
   * @param mapper Маппер классов.
   * @throws std::runtime_error если сеть не загружена.
   */
  YoloDetector(const Config& cfg, std::shared_ptr<IClassMapper> mapper);

  /**
   * @brief Детектирует объекты на изображении.
   * @param image Входное изображение.
   * @return Вектор обнаруженных объектов.
   */
  std::vector<Detection> detect(const cv::Mat& image) override;

 private:
  /**
   * @brief Обрабатывает выходные тензоры сети, применяет NMS и фильтрацию по
   * классам.
   * @param outs Выходные слои сети.
   * @param imgSize Размер исходного изображения.
   * @return Вектор детекций.
   */
  std::vector<Detection> processOutput(const std::vector<cv::Mat>& outs,
                                       const cv::Size& imgSize);

 private:
  cv::dnn::Net net_;     ///< Загруженная сеть YOLO
  cv::Size inputSize_;   ///< Размер входа сети
  float confThreshold_;  ///< Порог уверенности
  float nmsThreshold_;   ///< Порог NMS
  std::vector<int> allowedIds_;  ///< Идентификаторы разрешённых классов
};
