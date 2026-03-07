#include "polygons/Polygons.h"

#include <memory>

#include "interfaces/IFileReader.h"
#include "interfaces/IJsonParser.h"
#include "interfaces/IPolygonParser.h"
#include "polygons/FileReader.h"
#include "polygons/JsonParser.h"
#include "polygons/PolygonParser.h"

Polygons::Polygons(std::unique_ptr<IFileReader> fileReader,
                   std::unique_ptr<IJsonParser> jsonParser,
                   std::unique_ptr<IPolygonParser> polygonParser)
    : file_reader_(std::move(fileReader)),
      json_parser_(std::move(jsonParser)),
      polygon_parser_(std::move(polygonParser)) {}

Polygons::Polygons()
    : file_reader_(std::make_unique<FileReader>()),
      json_parser_(std::make_unique<JsonParser>()),
      polygon_parser_(std::make_unique<PolygonParser>()) {}

bool Polygons::LoadFromFile(const std::string& filename) {
  last_file_name_ = filename;
  auto content = file_reader_->Read(filename);
  if (!content) {
    return false;
  }
  auto json = json_parser_->Parse(*content);
  if (!json) {
    return false;
  }
  auto polygonsOpt = polygon_parser_->Parse(*json);
  if (!polygonsOpt) {
    return false;
  }
  polygons_ = std::move(*polygonsOpt);
  return true;
}