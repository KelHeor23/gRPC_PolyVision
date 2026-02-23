#pragma once
#include <boost/json.hpp>
#include <optional>
#include <vector>

#include "interfaces/IFileReader.h"
#include "interfaces/IJsonParser.h"
#include "interfaces/IPolygonParser.h"
#include "proto/ImageAnalysis.grpc.pb.h"

using ImageDetection::Polygon;

class Polygons {
 public:
  // Конструктор с внедрением зависимостей
  Polygons(std::unique_ptr<IFileReader> fileReader,
           std::unique_ptr<IJsonParser> jsonParser,
           std::unique_ptr<IPolygonParser> polygonParser);

  Polygons();

  // Основной метод: загружает и парсит полигоны из файла
  bool loadFromFile(const std::string& filename);

  const std::vector<Polygon>& getPolygons() const { return polygons_; }

 private:
  std::vector<Polygon> polygons_;
  std::unique_ptr<IFileReader> fileReader_;
  std::unique_ptr<IJsonParser> jsonParser_;
  std::unique_ptr<IPolygonParser> polygonParser_;
};