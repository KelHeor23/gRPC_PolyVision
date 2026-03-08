#pragma once
/**
 * @file yolo_detector.h
 * @brief Реализация детектора на базе YOLO (Darknet).
 */
#include "config.h"
#include "interfaces/i_detector.h"

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
   * @param class_names Набор имен детектируемых объектов
   */
  std::vector<Detection> Detect(
      const cv::Mat& image,
      const std::vector<std::string>& class_names) override;

 private:
  /**
   * @brief Обрабатывает выходные тензоры сети, применяет NMS и фильтрацию по
   * классам.
   * @param outs Выходные слои сети.
   * @param img_size Размер исходного изображения.
   * @return Вектор детекций.
   */
  std::vector<Detection> ProcessOutput(const std::vector<cv::Mat>& outs,
                                       const cv::Size& img_size);

 private:
  cv::dnn::Net net_;      ///< Загруженная сеть YOLO
  cv::Size input_size_;   ///< Размер входа сети
  float conf_threshold_;  ///< Порог уверенности
  float nms_threshold_;   ///< Порог NMS
  std::vector<int> allowed_ids_;  ///< Идентификаторы разрешённых классов
  std::shared_ptr<IClassMapper>
      mapper_;  ///< Класс работающий со списком детектируемых объектов в модели
};
