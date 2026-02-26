# Ищем Google Test
find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS json)

# Создаём исполняемый файл с тестами
add_executable(run_tests 
    client/FileReaderTest.cpp
    client/JsonParserTest.cpp
    client/PolygonParserTest.cpp
    client/PolygonsTest.cpp
    common/OpenCVEncoderTest.cpp
    server/ClassMapperTest.cpp
    server/PolygonProcessorTest.cpp
    ${CMAKE_SOURCE_DIR}/src/client/src/polygons/JsonParser.cpp
    ${CMAKE_SOURCE_DIR}/src/client/src/polygons/FileReader.cpp
    ${CMAKE_SOURCE_DIR}/src/client/src/polygons/PolygonParser.cpp
    ${CMAKE_SOURCE_DIR}/src/client/src/polygons/Polygons.cpp
    ${CMAKE_SOURCE_DIR}/src/server/src/impl/ClassMapper.cpp
    ${CMAKE_SOURCE_DIR}/src/server/src/impl/PolygonProcessor.cpp
    ${CMAKE_SOURCE_DIR}/src/server/src/processing/ImageProcessingServer.cpp
    )

target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/client/include 
    ${CMAKE_SOURCE_DIR}/src/server/include)

# Линкуем необходимые библиотеки проекта
target_link_libraries(run_tests
    PRIVATE
        GTest::gtest_main
        proto_generated
        common_lib             
        ${OpenCV_LIBS}
        protobuf::libprotobuf
        gRPC::grpc++
        Boost::json 
)

gtest_discover_tests(run_tests)