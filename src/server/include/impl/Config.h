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
  std::string model_weights;  ///< Путь к файлу весов YOLO ( .weights)
  std::string model_config;  ///< Путь к файлу конфигурации сети ( .cfg)
  std::string classes_file;  ///< Путь к файлу с именами классов (coco.names)
  float conf_threshold = 0.2f;  ///< Порог уверенности для детекций
  float nms_threshold = 0.4f;  ///< Порог NMS (Non-Maximum Suppression)
  cv::Size input_size = {640, 640};  ///< Размер входного изображения для сети
  std::vector<std::string> allowed_classes = {
      "orange", "apple", "person", "cat",
      "dog"};  ///< Список классов, которые нужно обрабатывать
};