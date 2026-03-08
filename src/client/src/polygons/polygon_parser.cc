#include "polygons/polygon_parser.h"

std::optional<ImageDetection::PolygonList> PolygonParser::Parse(
    const boost::json::value& root) {
  if (!root.is_object()) {
    return std::nullopt;
  }
  const auto& obj = root.as_object();

  ImageDetection::PolygonList polygon_list;

  // Поле "class_names" (опционально)
  auto class_names_it = obj.find("class_names");
  if (class_names_it != obj.end()) {
    auto class_names_opt = ParseClassNames(class_names_it->value());
    if (!class_names_opt) {
      return std::nullopt;  // неверный формат class_names
    }
    *polygon_list.mutable_class_names() = std::move(*class_names_opt);
  }

  // Поле "polygons" (обязательно)
  auto polygons_it = obj.find("polygons");
  if (polygons_it == obj.end() || !polygons_it->value().is_array()) {
    return std::nullopt;
  }
  const auto& polyArray = polygons_it->value().as_array();

  for (std::size_t i = 0; i < polyArray.size(); ++i) {
    auto polyOpt = ParseSinglePolygon(polyArray[i], i);
    if (!polyOpt) {
      // Пропускаем невалидный полигон
      continue;
    }
    *polygon_list.add_polygons() = std::move(*polyOpt);
  }

  if (polygon_list.polygons_size() == 0) {
    return std::nullopt;  // нет ни одного валидного полигона
  }

  return polygon_list;
}

std::optional<ImageDetection::Polygon> PolygonParser::ParseSinglePolygon(
    const boost::json::value& val, std::size_t index) {
  if (!val.is_object()) return std::nullopt;

  const auto& obj = val.as_object();

  ImageDetection::Polygon poly;

  // scalar fields
  auto typeIt = obj.find("type");
  auto priorityIt = obj.find("priority");
  auto thresholdIt = obj.find("threshold");

  if (typeIt == obj.end() || priorityIt == obj.end() ||
      thresholdIt == obj.end())
    return std::nullopt;

  if (!typeIt->value().is_int64() || !priorityIt->value().is_int64() ||
      !thresholdIt->value().is_double())
    return std::nullopt;

  int typeVal = static_cast<int>(typeIt->value().as_int64());

  if (typeVal != 0 && typeVal != 1) return std::nullopt;

  double thresh = thresholdIt->value().as_double();

  if (thresh < 0.0 || thresh > 1.0) return std::nullopt;

  poly.set_type(static_cast<ImageDetection::PolygonType>(typeVal));
  poly.set_priority(static_cast<int>(priorityIt->value().as_int64()));
  poly.set_threshold(static_cast<float>(thresh));

  // points
  auto pointsIt = obj.find("points");
  if (pointsIt == obj.end() || !pointsIt->value().is_array())
    return std::nullopt;
  const auto& pointsArray = pointsIt->value().as_array();
  if (pointsArray.size() < 3) return std::nullopt;

  for (const auto& ptVal : pointsArray) {
    if (!ptVal.is_object()) return std::nullopt;
    const auto& ptObj = ptVal.as_object();
    auto xIt = ptObj.find("x");
    auto yIt = ptObj.find("y");
    if (xIt == ptObj.end() || yIt == ptObj.end()) return std::nullopt;
    if (!xIt->value().is_int64() || !yIt->value().is_int64())
      return std::nullopt;
    auto* point = poly.add_points();
    point->set_x(static_cast<int32_t>(xIt->value().as_int64()));
    point->set_y(static_cast<int32_t>(yIt->value().as_int64()));
  }
  return poly;
}

std::optional<google::protobuf::RepeatedPtrField<std::string>>
PolygonParser::ParseClassNames(const boost::json::value& val) {
  if (!val.is_array()) return std::nullopt;

  const auto& arr = val.as_array();
  google::protobuf::RepeatedPtrField<std::string> result;
  for (const auto& elem : arr) {
    if (!elem.is_string()) return std::nullopt;
    result.Add(elem.as_string().c_str());
  }
  return result;
}