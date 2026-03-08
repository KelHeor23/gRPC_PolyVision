#pragma once
/**
 * @file polygons.h
 * @brief Высокоуровневый класс для загрузки и хранения PolygonList из
 * JSON-файла.
 */
#include <boost/json.hpp>
#include <optional>
#include <vector>

#include "interfaces/i_file_reader.h"
#include "interfaces/i_json_parser.h"
#include "interfaces/i_polygon_parser.h"
#include "proto/ImageAnalysis.grpc.pb.h"

using ImageDetection::PolygonList;

/**
 * @class Polygons
 * @brief Управляет загрузкой и доступом к набору полигонов и метаданным.
 *
 * Класс использует внедрённые зависимости для чтения файла, парсинга JSON
 * и преобразования в protobuf-сообщение PolygonList.
 */
class Polygons {
 public:
  /**
   * @brief Конструктор с явным внедрением зависимостей.
   * @param fileReader Объект для чтения файлов.
   * @param jsonParser Парсер JSON.
   * @param polygonParser Парсер полигонов (возвращает PolygonList).
   */
  Polygons(std::unique_ptr<IFileReader> fileReader,
           std::unique_ptr<IJsonParser> jsonParser,
           std::unique_ptr<IPolygonParser> polygonParser);

  /**
   * @brief Конструктор по умолчанию, использующий стандартные реализации.
   */
  Polygons();

  /**
   * @brief Загружает и парсит данные из указанного файла.
   * @param filename Путь к JSON-файлу.
   * @return true при успешной загрузке, false при ошибке.
   */
  bool LoadFromFile(const std::string& filename);

  /**
   * @brief Возвращает загруженный PolygonList.
   * @return Константная ссылка на PolygonList.
   */
  const PolygonList& GetPolygonList() const { return polygon_list_; }

  /**
   * @brief Имя последнего загруженного файла.
   * @return Строка с именем файла.
   */
  const std::string& GetLastFileName() const { return last_file_name_; }

 private:
  std::string last_file_name_;  ///< Имя последнего загруженного файла
  PolygonList polygon_list_;  ///< Загруженные данные (полигоны + метаданные)
  std::unique_ptr<IFileReader> file_reader_;  ///< Читатель файлов
  std::unique_ptr<IJsonParser> json_parser_;  ///< Парсер JSON
  std::unique_ptr<IPolygonParser>
      polygon_parser_;  ///< Парсер полигонов (возвращает PolygonList)
};