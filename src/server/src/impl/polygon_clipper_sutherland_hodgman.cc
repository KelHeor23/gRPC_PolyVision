#include "impl/polygon_clipper_sutherland_hodgman.h"

std::vector<ImageDetection::Point> SutherlandHodgmanClipper::GetInternalPolygon(
    std::vector<ImageDetection::Point> polygon, const cv::Rect& box) {
  const Edge edges[4] = {LEFT, RIGHT, TOP, BOTTOM};
  borders_ = {.left = box.x,
              .right = box.x + box.width,
              .top = box.y,
              .bottom = box.y + box.height};

  std::vector<ImageDetection::Point> temp_poly_buffer;
  temp_poly_buffer.reserve(polygon.size() * 2);

  for (auto& edge : edges) {
    if (polygon.empty()) break;
    temp_poly_buffer.clear();
    for (size_t i = 0; i < polygon.size(); i++) {
      const ImageDetection::Point& cur = polygon[i];
      const ImageDetection::Point& next = polygon[(i + 1) % polygon.size()];

      bool cur_inside = IsInside(cur, edge);
      bool next_inside = IsInside(next, edge);

      if (cur_inside) {
        if (next_inside) {
          // Оба внутри – добавляем следующую вершину
          temp_poly_buffer.push_back(next);
        } else {
          // Выход изнутри наружу – добавляем точку пересечения
          temp_poly_buffer.push_back(GetIntersectionPoint(cur, next, edge));
        }
      } else {
        if (next_inside) {
          // Вход снаружи внутрь – добавляем пересечение и следующую вершину
          temp_poly_buffer.push_back(GetIntersectionPoint(cur, next, edge));
          temp_poly_buffer.push_back(next);
        }
        // Оба снаружи – ничего не добавляем
      }
    }
    polygon.swap(temp_poly_buffer);
  }

  return polygon;
}

bool SutherlandHodgmanClipper::IsInside(const ImageDetection::Point& p,
                                        SutherlandHodgmanClipper::Edge edge) {
  switch (edge) {
    case LEFT:
      return p.x() >= borders_.left;
    case RIGHT:
      return p.x() <= borders_.right;
    case TOP:
      return p.y() >= borders_.top;
    case BOTTOM:
      return p.y() <= borders_.bottom;
  }
  return false;
}

ImageDetection::Point SutherlandHodgmanClipper::GetIntersectionPoint(
    const ImageDetection::Point& p1, const ImageDetection::Point& p2,
    Edge edge) const {
  // Если отрезок вертикальный/горизонтальный и лежит на границе, возвращаем
  // одну из точек
  if ((edge == LEFT || edge == RIGHT) && p2.x() == p1.x()) return p1;
  if ((edge == TOP || edge == BOTTOM) && p2.y() == p1.y()) return p1;

  ImageDetection::Point res;
  double t, x, y;

  switch (edge) {
    case LEFT:
      t = double(borders_.left - p1.x()) / double(p2.x() - p1.x());
      x = borders_.left;
      y = p1.y() + t * (p2.y() - p1.y());
      break;

    case RIGHT:
      t = double(borders_.right - p1.x()) / double(p2.x() - p1.x());
      x = borders_.right;
      y = p1.y() + t * (p2.y() - p1.y());
      break;

    case TOP:
      t = double(borders_.top - p1.y()) / double(p2.y() - p1.y());
      y = borders_.top;
      x = p1.x() + t * (p2.x() - p1.x());
      break;

    case BOTTOM:
      t = double(borders_.bottom - p1.y()) / double(p2.y() - p1.y());
      y = borders_.bottom;
      x = p1.x() + t * (p2.x() - p1.x());
      break;

    default:
      return res;
  }

  res.set_x(cvRound(x));
  res.set_y(cvRound(y));
  return res;
}