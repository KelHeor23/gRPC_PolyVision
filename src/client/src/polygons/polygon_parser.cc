#include "polygons/polygon_parser.h"

std::optional<ImageDetection::PolygonList> PolygonParser::Parse(
    const boost::json::value& root) const {
  if (!root.is_object()) {
    return std::nullopt;
  }
  const auto& obj = root.as_object();

  auto polygons_it = obj.find("polygons");
  if (polygons_it == obj.end() || !polygons_it->value().is_array()) {
    return std::nullopt;
  }
  const auto& polyArray = polygons_it->value().as_array();

  ImageDetection::PolygonList polygon_list;

  for (std::size_t i = 0; i < polyArray.size(); ++i) {
    auto polyOpt = ParseSinglePolygon(polyArray[i], i);
    if (!polyOpt) {
      continue;  // пропускаем невалидный полигон
    }
    *polygon_list.add_polygons() = std::move(*polyOpt);
  }

  if (polygon_list.polygons_size() == 0) {
    return std::nullopt;
  }

  return polygon_list;
}

std::optional<ImageDetection::Polygon> PolygonParser::ParseSinglePolygon(
    const boost::json::value& val, std::size_t index) const {
  if (!val.is_object()) return std::nullopt;

  const auto& obj = val.as_object();

  ImageDetection::Polygon poly;

  // type, priority, threshold
  auto type_it = obj.find("type");
  auto priority_it = obj.find("priority");
  auto threshold_it = obj.find("threshold");

  if (type_it == obj.end() || priority_it == obj.end() ||
      threshold_it == obj.end())
    return std::nullopt;

  if (!type_it->value().is_int64() || !priority_it->value().is_int64() ||
      !threshold_it->value().is_double())
    return std::nullopt;

  int type_val = static_cast<int>(type_it->value().as_int64());
  if (type_val != 0 && type_val != 1) return std::nullopt;

  double thresh = threshold_it->value().as_double();
  if (thresh < 0.0 || thresh > 1.0) return std::nullopt;

  poly.set_type(static_cast<ImageDetection::PolygonType>(type_val));
  poly.set_priority(static_cast<int>(priority_it->value().as_int64()));
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

  // class_names (обязательное поле внутри полигона)
  auto class_names_it = obj.find("class_names");
  if (class_names_it == obj.end()) return std::nullopt;
  if (!class_names_it->value().is_array()) return std::nullopt;
  const auto& classArray = class_names_it->value().as_array();
  for (const auto& cls_val : classArray) {
    if (!cls_val.is_string()) return std::nullopt;
    poly.add_class_names(cls_val.as_string().c_str());
  }

  return poly;
}