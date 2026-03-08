#include "polygons/polygons.h"

#include <memory>

#include "interfaces/i_file_reader.h"
#include "interfaces/i_json_parser.h"
#include "interfaces/i_polygon_parser.h"
#include "polygons/file_reader.h"
#include "polygons/json_parser.h"
#include "polygons/polygon_parser.h"

Polygons::Polygons(std::unique_ptr<IFileReader> file_reader,
                   std::unique_ptr<IJsonParser> json_parser,
                   std::unique_ptr<IPolygonParser> polygon_parser)
    : file_reader_(std::move(file_reader)),
      json_parser_(std::move(json_parser)),
      polygon_parser_(std::move(polygon_parser)) {}

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
  auto polygon_list_opt = polygon_parser_->Parse(*json);
  if (!polygon_list_opt) {
    return false;
  }
  polygon_list_ = std::move(*polygon_list_opt);
  return true;
}