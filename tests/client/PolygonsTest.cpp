#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "polygons/FileReader.h"
#include "polygons/JsonParser.h"
#include "polygons/PolygonParser.h"
#include "polygons/Polygons.h"
#include "proto/ImageAnalysis.pb.h"

class PolygonsRealTest : public ::testing::Test {
 protected:
  // Вспомогательная функция для создания временного файла с заданным содержимым
  std::filesystem::path createTempFile(const std::string& content) {
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    std::srand(std::time(nullptr));
    std::string filename =
        "test_polygons_" + std::to_string(std::rand()) + ".json";
    std::filesystem::path filePath = tempDir / filename;
    std::ofstream ofs(filePath);
    ofs << content;
    ofs.close();
    return filePath;
  }

  void TearDown() override {
    for (const auto& file : tempFiles) {
      std::filesystem::remove(file);
    }
  }

  std::vector<std::filesystem::path> tempFiles;
};

// Успешная загрузка корректного JSON с двумя полигонами
TEST_F(PolygonsRealTest, LoadValidFileSuccess) {
  std::string jsonContent = R"({
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

  std::filesystem::path filePath = createTempFile(jsonContent);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());

  EXPECT_TRUE(result);
  EXPECT_EQ(polygons.getLastFileName(), filePath.string());
  const auto& polyVec = polygons.getPolygons();
  ASSERT_EQ(polyVec.size(), 2);

  const auto& p0 = polyVec[0];
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

  const auto& p1 = polyVec[1];
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
  bool result = polygons.loadFromFile("non_existent_file.json");
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.getPolygons().empty());
}

// Некорректный JSON (синтаксическая ошибка)
TEST_F(PolygonsRealTest, InvalidJsonSyntax) {
  std::string badJson =
      R"({"polygons": [{"type":0, "priority":1, "threshold":0.5, "points":[{"x":10,"y":10}] )";  // незакрытая скобка
  std::filesystem::path filePath = createTempFile(badJson);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.getPolygons().empty());
}

// JSON корректен, но не содержит поля "polygons"
TEST_F(PolygonsRealTest, MissingPolygonsField) {
  std::string jsonContent = R"({"other": []})";
  std::filesystem::path filePath = createTempFile(jsonContent);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.getPolygons().empty());
}

// Поле polygons есть, но оно не массив
TEST_F(PolygonsRealTest, PolygonsNotArray) {
  std::string jsonContent = R"({"polygons": "not an array"})";
  std::filesystem::path filePath = createTempFile(jsonContent);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.getPolygons().empty());
}

// Все полигоны в массиве невалидны (например, недостаточно точек)
TEST_F(PolygonsRealTest, AllPolygonsInvalid) {
  std::string jsonContent = R"({
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
  std::filesystem::path filePath = createTempFile(jsonContent);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());
  EXPECT_FALSE(result);
  EXPECT_TRUE(polygons.getPolygons().empty());
}

// Смесь валидных и невалидных полигонов – должны загрузиться только валидные
TEST_F(PolygonsRealTest, MixedValidInvalidPolygons) {
  std::string jsonContent = R"({
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
  auto jsonVal = boost::json::parse(jsonContent);
  PolygonParser parser;
  auto directResult = parser.parse(jsonVal);
  ASSERT_TRUE(directResult.has_value())
      << "PolygonParser вернул nullopt, хотя ожидались валидные полигоны";
  EXPECT_EQ(directResult->size(), 2) << "PolygonParser вернул не 2 полигона";

  // Теперь проверяем загрузку через файл
  std::filesystem::path filePath = createTempFile(jsonContent);
  tempFiles.push_back(filePath);

  Polygons polygons;
  bool result = polygons.loadFromFile(filePath.string());

  EXPECT_TRUE(result);
  const auto& polyVec = polygons.getPolygons();
  ASSERT_EQ(polyVec.size(), 2);

  // Первый валидный полигон (3 точки)
  EXPECT_EQ(polyVec[0].type(), 0);
  EXPECT_EQ(polyVec[0].priority(), 1);
  EXPECT_FLOAT_EQ(polyVec[0].threshold(), 0.5f);
  ASSERT_EQ(polyVec[0].points_size(), 3);
  EXPECT_EQ(polyVec[0].points(0).x(), 10);
  EXPECT_EQ(polyVec[0].points(0).y(), 10);
  EXPECT_EQ(polyVec[0].points(1).x(), 20);
  EXPECT_EQ(polyVec[0].points(1).y(), 10);
  EXPECT_EQ(polyVec[0].points(2).x(), 20);
  EXPECT_EQ(polyVec[0].points(2).y(), 20);

  // Второй валидный полигон (4 точки)
  EXPECT_EQ(polyVec[1].type(), 0);
  EXPECT_EQ(polyVec[1].priority(), 3);
  EXPECT_FLOAT_EQ(polyVec[1].threshold(), 0.9f);
  ASSERT_EQ(polyVec[1].points_size(), 4);
  EXPECT_EQ(polyVec[1].points(0).x(), 50);
  EXPECT_EQ(polyVec[1].points(0).y(), 50);
  EXPECT_EQ(polyVec[1].points(1).x(), 60);
  EXPECT_EQ(polyVec[1].points(1).y(), 50);
  EXPECT_EQ(polyVec[1].points(2).x(), 60);
  EXPECT_EQ(polyVec[1].points(2).y(), 60);
  EXPECT_EQ(polyVec[1].points(3).x(), 50);
  EXPECT_EQ(polyVec[1].points(3).y(), 60);
}