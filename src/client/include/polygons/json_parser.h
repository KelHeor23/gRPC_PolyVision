#pragma once
/**
 * @file json_parser.h
 * @brief Реализация IJsonParser на базе Boost.JSON.
 */
#include <boost/json.hpp>

#include "interfaces/i_json_parser.h"

/**
 * @class JsonParser
 * @brief Парсит строку JSON в объект Boost.JSON.
 */
class JsonParser : public IJsonParser {
 public:
  /**
   * @brief Пытается разобрать строку как JSON.
   * @param json Входная строка.
   * @return Объект JSON или nullopt при ошибке.
   */
  std::optional<boost::json::value> Parse(
      const std::string& json) const override;
};