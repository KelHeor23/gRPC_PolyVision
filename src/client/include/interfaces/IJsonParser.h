#pragma once
/**
 * @file IJsonParser.h
 * @brief Интерфейс для парсинга JSON-строк.
 */
#include <boost/json.hpp>
#include <optional>
#include <string>

/**
 * @interface IJsonParser
 * @brief Преобразует строку JSON в объект Boost.JSON.
 */
class IJsonParser {
 public:
  virtual ~IJsonParser() = default;
  /**
   * @brief Парсит JSON-строку.
   * @param json Строка с JSON-данными.
   * @return Значение Boost.JSON или std::nullopt при ошибке парсинга.
   */
  virtual std::optional<boost::json::value> parse(const std::string& json) = 0;
};