#pragma once
/**
 * @file polygon.h
 * @brief Структура, хранящая полигон и объекты, которые он детектирует
 */
#include <vector>

#include "proto/ImageAnalysis.grpc.pb.h"

/**
 * @struct Polygon
 * @brief Содержит информацию о полигоне и найденных объектах.
 */
struct Polygon {
  ImageDetection::Polygon polygon;  ///< Полиггон
  std::vector<int> allowed_id;  ///< id объектов которые он детектирует
};