#include "polygons/polygons.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "polygons/file_reader.h"
#include "polygons/json_parser.h"
#include "polygons/polygon_parser.h"
#include "proto/ImageAnalysis.pb.h"

class PolygonsRealTest : public ::testing::Test {
 protected:
  // Вспомогательная функция для создания временного файла с заданным содержимым
  std::filesystem::path CreateTempFile(const std::string& content) {
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
    std::srand(std::time(nullptr));
    std::string file_name =
        "test_polygons_" + std::to_string(std::rand()) + ".json";
    std::filesystem::path file_path = temp_dir / file_name;
    std::ofstream ofs(file_path);
    ofs << content;
    ofs.close();
    return file_path;
  }

  void TearDown() override {
    for (const auto& file : temp_files_) {
      std::filesystem::remove(file);
    }
  }

  std::vector<std::filesystem::path> temp_files_;
};

// Успешная загрузка корректного JSON с двумя полигонами
TEST_F(PolygonsRealTest, LoadValidFileSuccess) {
  std::string json_content = R"({
        "polygons": [
            {
                "type": 0,
                "priority": 1,
                "threshold": 0.5,
                "points": [{"x":10,"y":10}, {"x":20,"y":10}, {"x":20,"y":20}, {"x":10,"y":20}]
            },
            {
                "type": 1,
                "priority": 2,
                "threshold": 0.8,
                "points": [{"x":30,"y":30}, {"x":40,"y":30}, {"x":40,"y":40}]
            }
        ]
    })";

  std::filesystem::path file_path = CreateTempFile(json_content);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());

  EXPECT_TRUE(result);
  EXPECT_EQ(polygons.GetLastFileName(), file_path.string());
  const auto& poly_vec = polygons.GetPolygons();
  ASSERT_EQ(poly_vec.size(), 2);

  const auto& p0 = poly_vec[0];
  EXPECT_EQ(p0.type(), 0);
  EXPECT_EQ(p0.priority(), 1);
  EXPECT_FLOAT_EQ(p0.threshold(), 0.5f);
  ASSERT_EQ(p0.points_size(), 4);
  EXPECT_EQ(p0.points(0).x(), 10);
  EXPECT_EQ(p0.points(0).y(), 10);
  EXPECT_EQ(p0.points(1).x(), 20);
  EXPECT_EQ(p0.points(1).y(), 10);
  EXPECT_EQ(p0.points(2).x(), 20);
  EXPECT_EQ(p0.points(2).y(), 20);
  EXPECT_EQ(p0.points(3).x(), 10);
  EXPECT_EQ(p0.points(3).y(), 20);

  const auto& p1 = poly_vec[1];
  EXPECT_EQ(p1.type(), 1);
  EXPECT_EQ(p1.priority(), 2);
  EXPECT_FLOAT_EQ(p1.threshold(), 0.8f);
  ASSERT_EQ(p1.points_size(), 3);
  EXPECT_EQ(p1.points(0).x(), 30);
  EXPECT_EQ(p1.points(0).y(), 30);
  EXPECT_EQ(p1.points(1).x(), 40);
  EXPECT_EQ(p1.points(1).y(), 30);
  EXPECT_EQ(p1.points(2).x(), 40);
  EXPECT_EQ(p1.points(2).y(), 40);
}

// Файл не существует
TEST_F(PolygonsRealTest, FileNotFound) {
  Polygons polygons;
  bool result = polygons.LoadFromFile("non_existent_file.json");
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.GetPolygons().empty());
}

// Некорректный JSON (синтаксическая ошибка)
TEST_F(PolygonsRealTest, InvalidJsonSyntax) {
  std::string bad_json =
      R"({"polygons": [{"type":0, "priority":1, "threshold":0.5, "points":[{"x":10,"y":10}] )";  // незакрытая скобка
  std::filesystem::path file_path = CreateTempFile(bad_json);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.GetPolygons().empty());
}

// JSON корректен, но не содержит поля "polygons"
TEST_F(PolygonsRealTest, MissingPolygonsField) {
  std::string json_content = R"({"other": []})";
  std::filesystem::path file_path = CreateTempFile(json_content);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.GetPolygons().empty());
}

// Поле polygons есть, но оно не массив
TEST_F(PolygonsRealTest, PolygonsNotArray) {
  std::string json_content = R"({"polygons": "not an array"})";
  std::filesystem::path file_path = CreateTempFile(json_content);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.GetPolygons().empty());
}

// Все полигоны в массиве невалидны (например, недостаточно точек)
TEST_F(PolygonsRealTest, AllPolygonsInvalid) {
  std::string json_content = R"({
        "polygons": [
            {
                "type": 0,
                "priority": 1,
                "threshold": 0.5,
                "points": [{"x":10,"y":10}, {"x":20,"y":10}]  // только 2 точки
            },
            {
                "type": 1,
                "priority": 2,
                "threshold": 0.8,
                "points": [{"x":30,"y":30}]  // только 1 точка
            }
        ]
    })";
  std::filesystem::path file_path = CreateTempFile(json_content);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.GetPolygons().empty());
}

// Смесь валидных и невалидных полигонов – должны загрузиться только валидные
TEST_F(PolygonsRealTest, MixedValidInvalidPolygons) {
  std::string json_content = R"({
        "polygons": [
            {
                "type": 0,
                "priority": 1,
                "threshold": 0.5,
                "points": [{"x":10,"y":10}, {"x":20,"y":10}, {"x":20,"y":20}]
            },
            {
                "type": 1,
                "priority": 2,
                "threshold": 0.8,
                "points": [{"x":30,"y":30}, {"x":40,"y":30}]
            },
            {
                "type": 0,
                "priority": 3,
                "threshold": 0.9,
                "points": [{"x":50,"y":50}, {"x":60,"y":50}, {"x":60,"y":60}, {"x":50,"y":60}]
            }
        ]
    })";

  // Проверяем напрямую работу PolygonParser (для локализации ошибки)
  auto json_val = boost::json::parse(json_content);
  PolygonParser parser;
  auto direct_result = parser.Parse(json_val);
  ASSERT_TRUE(direct_result.has_value())
      << "PolygonParser вернул nullopt, хотя ожидались валидные полигоны";
  EXPECT_EQ(direct_result->size(), 2) << "PolygonParser вернул не 2 полигона";

  // Теперь проверяем загрузку через файл
  std::filesystem::path file_path = CreateTempFile(json_content);
  temp_files_.push_back(file_path);

  Polygons polygons;
  bool result = polygons.LoadFromFile(file_path.string());

  EXPECT_TRUE(result);
  const auto& poly_vec = polygons.GetPolygons();
  ASSERT_EQ(poly_vec.size(), 2);

  // Первый валидный полигон (3 точки)
  EXPECT_EQ(poly_vec[0].type(), 0);
  EXPECT_EQ(poly_vec[0].priority(), 1);
  EXPECT_FLOAT_EQ(poly_vec[0].threshold(), 0.5f);
  ASSERT_EQ(poly_vec[0].points_size(), 3);
  EXPECT_EQ(poly_vec[0].points(0).x(), 10);
  EXPECT_EQ(poly_vec[0].points(0).y(), 10);
  EXPECT_EQ(poly_vec[0].points(1).x(), 20);
  EXPECT_EQ(poly_vec[0].points(1).y(), 10);
  EXPECT_EQ(poly_vec[0].points(2).x(), 20);
  EXPECT_EQ(poly_vec[0].points(2).y(), 20);

  // Второй валидный полигон (4 точки)
  EXPECT_EQ(poly_vec[1].type(), 0);
  EXPECT_EQ(poly_vec[1].priority(), 3);
  EXPECT_FLOAT_EQ(poly_vec[1].threshold(), 0.9f);
  ASSERT_EQ(poly_vec[1].points_size(), 4);
  EXPECT_EQ(poly_vec[1].points(0).x(), 50);
  EXPECT_EQ(poly_vec[1].points(0).y(), 50);
  EXPECT_EQ(poly_vec[1].points(1).x(), 60);
  EXPECT_EQ(poly_vec[1].points(1).y(), 50);
  EXPECT_EQ(poly_vec[1].points(2).x(), 60);
  EXPECT_EQ(poly_vec[1].points(2).y(), 60);
  EXPECT_EQ(poly_vec[1].points(3).x(), 50);
  EXPECT_EQ(poly_vec[1].points(3).y(), 60);
}