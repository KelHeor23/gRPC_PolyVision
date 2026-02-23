# Исполняемый файл клиента
add_executable(client main.cpp 
    src/polygons/polygons.cpp 
    src/polygons/FileReader.cpp
    src/polygons/JsonParser.cpp
    src/polygons/PolygonParser.cpp
#target_include_directories(client PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../common   # если нужны общие утилиты
#)
target_include_directories(client PRIVATE include)

# Ищем Boost с компонентом json
find_package(Boost REQUIRED COMPONENTS json)

if(Boost_FOUND)
    message(STATUS "Boost found. Include dirs: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost (with JSON) not found. Please install Boost >= 1.75")
endif()

# Линкуем библиотеки
target_link_libraries(client
    proto_generated
    ${OpenCV_LIBS}
    Boost::json
)