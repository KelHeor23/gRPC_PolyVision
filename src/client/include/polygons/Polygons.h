#pragma once
/**
 * @file Polygons.h
 * @brief Высокоуровневый класс для загрузки и хранения полигонов из JSON-файла.
 */
#include <boost/json.hpp>
#include <optional>
#include <vector>

#include "interfaces/IFileReader.h"
#include "interfaces/IJsonParser.h"
#include "interfaces/IPolygonParser.h"
#include "proto/ImageAnalysis.grpc.pb.h"

using ImageDetection::Polygon;

/**
 * @class Polygons
 * @brief Управляет загрузкой и доступом к набору полигонов.
 *
 * Класс использует внедрённые зависимости для чтения файла, парсинга JSON
 * и преобразования в protobuf-объекты. Может работать с реализациями по
 * умолчанию.
 */
class Polygons {
 public:
  /**
   * @brief Конструктор с явным внедрением зависимостей.
   * @param fileReader Объект для чтения файлов.
   * @param jsonParser Парсер JSON.
   * @param polygonParser Парсер полигонов.
   */
  Polygons(std::unique_ptr<IFileReader> fileReader,
           std::unique_ptr<IJsonParser> jsonParser,
           std::unique_ptr<IPolygonParser> polygonParser);

  /**
   * @brief Конструктор по умолчанию, использующий стандартные реализации.
   */
  Polygons();

  /**
   * @brief Загружает и парсит полигоны из указанного файла.
   * @param filename Путь к JSON-файлу.
   * @return true при успешной загрузке, false при ошибке.
   */
  bool loadFromFile(const std::string& filename);

  /**
   * @brief Возвращает загруженные полигоны.
   * @return Константная ссылка на вектор полигонов.
   */
  const std::vector<Polygon>& getPolygons() const { return polygons_; }
  /**
   * @brief Имя последнего загруженного файла.
   * @return Строка с именем файла.
   */
  const std::string& getLastFileName() const { return lastFileName_; }

 private:
  std::string lastFileName_;  ///< Имя последнего загруженного файла
  std::vector<Polygon> polygons_;  ///< Загруженные полигоны
  std::unique_ptr<IFileReader> fileReader_;  ///< Читатель файлов
  std::unique_ptr<IJsonParser> jsonParser_;  ///< Парсер JSON
  std::unique_ptr<IPolygonParser> polygonParser_;  ///< Парсер полигонов
};