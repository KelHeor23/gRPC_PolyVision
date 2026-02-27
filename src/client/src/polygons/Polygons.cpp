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
    : fileReader_(std::move(fileReader)),
      jsonParser_(std::move(jsonParser)),
      polygonParser_(std::move(polygonParser)) {}

Polygons::Polygons()
    : fileReader_(std::make_unique<FileReader>()),
      jsonParser_(std::make_unique<JsonParser>()),
      polygonParser_(std::make_unique<PolygonParser>()) {}

bool Polygons::loadFromFile(const std::string& filename) {
  lastFileName_ = filename;
  auto content = fileReader_->read(filename);
  if (!content) {
    return false;
  }
  auto json = jsonParser_->parse(*content);
  if (!json) {
    return false;
  }
  auto polygonsOpt = polygonParser_->parse(*json);
  if (!polygonsOpt) {
    return false;
  }
  polygons_ = std::move(*polygonsOpt);
  return true;
}