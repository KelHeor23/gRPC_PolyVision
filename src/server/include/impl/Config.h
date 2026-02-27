#pragma once
/**
 * @file Config.h
 * @brief Структура конфигурации для детектора объектов.
 */
#include <opencv2/core.hpp>
#include <string>
#include <vector>

/**
 * @struct Config
 * @brief Параметры настройки YOLO и пост-обработки.
 */
struct Config {
  std::string modelWeights;  ///< Путь к файлу весов YOLO ( .weights)
  std::string modelConfig;  ///< Путь к файлу конфигурации сети ( .cfg)
  std::string classesFile;  ///< Путь к файлу с именами классов (coco.names)
  float confThreshold = 0.2f;  ///< Порог уверенности для детекций
  float nmsThreshold = 0.4f;  ///< Порог NMS (Non-Maximum Suppression)
  cv::Size inputSize = {640, 640};  ///< Размер входного изображения для сети
  std::vector<std::string> allowedClasses = {
      "orange", "apple", "person", "cat",
      "dog"};  ///< Список классов, которые нужно обрабатывать
};