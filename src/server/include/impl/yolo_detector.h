#pragma once
/**
 * @file yolo_detector.h
 * @brief Реализация детектора на базе YOLO (Darknet).
 */
#include <vector>

#include "config.h"
#include "interfaces/i_detector.h"

class IImagePreprocessor;
class IInferenceBackend;
class IDetectionOutputParser;

struct Detection;

/**
 * @class YoloDetector
 * @brief Оркестратор, объединяющий предобработку, инференс и парсинг.
 *
 * Этот класс следует принципу инверсии зависимостей, принимая абстракции
 * для каждого этапа. Он реализует интерфейс IDetector.
 */
class YoloDetector : public IDetector {
 public:
  /**
   * @brief Конструктор.
   * @param preprocessor Умный указатель на предобработчик изображений.
   * @param backend      Умный указатель на бэкенд инференса.
   * @param parser       Умный указатель на парсер выходов.
   * @param confThreshold Порог уверенности.
   * @param nmsThreshold  Порог NMS.
   */
  YoloDetector(std::unique_ptr<IImagePreprocessor> preprocessor,
               std::unique_ptr<IInferenceBackend> backend,
               std::unique_ptr<IDetectionOutputParser> parser,
               float confThreshold, float nmsThreshold);

  /**
   * @brief Выполняет детекцию объектов на изображении.
   * @param image Входное изображение в формате BGR.
   * @return std::vector<Detection> Список обнаруженных объектов.
   */
  std::vector<Detection> Detect(const cv::Mat& image) override;

 private:
  std::unique_ptr<IImagePreprocessor> preprocessor_;  ///< Предобработчик.
  std::unique_ptr<IInferenceBackend> backend_;  ///< Бэкенд инференса.
  std::unique_ptr<IDetectionOutputParser> parser_;  ///< Парсер выходов.
  float confThreshold_;  ///< Порог уверенности.
  float nmsThreshold_;   ///< Порог NMS.
};
