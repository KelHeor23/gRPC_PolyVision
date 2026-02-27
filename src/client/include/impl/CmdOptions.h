#pragma once
/**
 * @file CmdOptions.h
 * @brief Реализация обработки аргументов командной строки
 */

#include <boost/program_options.hpp>
#include <string>
#include <unordered_map>

namespace po = boost::program_options;

/**
 * @class CommandOptions
 * @brief Класс для парсинга и хранения параметров командной строки.
 *
 * Позволяет получить пути к файлу изображения и файлу полигонов, а также
 * проверить, был ли запрошен вывод справки.
 */
class CommandOptions {
 public:
  /**
   * @brief Конструктор, инициализирует описание допустимых опций.
   */
  CommandOptions();
  ~CommandOptions();

  /**
   * @brief Разбирает аргументы командной строки.
   * @param argc Количество аргументов.
   * @param argv Массив аргументов.
   * @throws std::runtime_error при ошибках разбора (неизвестные опции,
   * отсутствие обязательных значений и т.д.).
   */
  void Parse(int argc, char* argv[]);

  /**
   * @brief Возвращает путь к файлу изображения.
   * @return Строка с путём.
   */
  std::string GetImageFile() const { return imageFile_; }

  /**
   * @brief Возвращает путь к файлу полигонов.
   * @return Строка с путём.
   */
  std::string GetPolygonsFile() const { return polygonsFile_; }

  /**
   * @brief Проверяет, был ли запрошен вывод справки.
   * @return true, если указан флаг --help или -h.
   */
  bool isHelpRequested() const { return helpRequested_; }

  /**
   * @brief Выводит справку по использованию программы в std::cout.
   */
  void printHelp();

 private:
  std::string imageFile_;     ///< Путь к файлу изображения
  std::string polygonsFile_;  ///< Путь к файлу полигонов

  po::options_description desc_;  ///< Описание допустимых опций
  po::variables_map vm_;  ///< Хранилище разобранных значений
  bool helpRequested_ = false;  ///< Флаг запроса справки
};
