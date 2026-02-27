#pragma once
/**
 * @file Detection.h
 * @brief Структура, описывающая одно обнаружение объекта.
 */
#include <opencv2/opencv.hpp>

/**
 * @struct Detection
 * @brief Содержит информацию о найденном объекте.
 */
struct Detection {
  int classId;       ///< Идентификатор класса
  float confidence;  ///< Уверенность (0..1)
  cv::Rect box;      ///< Бокс, ограничивающий объект
};