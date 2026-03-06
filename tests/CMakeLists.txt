# Ищем Google Test
find_package(GTest REQUIRED)

# Создаём исполняемый файл с тестами
add_executable(run_tests 
    client/FileReaderTest.cpp
    client/JsonParserTest.cpp
    client/PolygonParserTest.cpp
    client/PolygonsTest.cpp
    common/OpenCVEncoderTest.cpp
    server/ClassMapperTest.cpp
    server/PolygonProcessorTest.cpp
    )

target_include_directories(run_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/client/include 
    ${CMAKE_SOURCE_DIR}/src/server/include)

# Линкуем необходимые библиотеки проекта
target_link_libraries(run_tests
    PRIVATE
        GTest::gtest_main
        proto_generated
        client_lib
        server_lib
        common_lib             
        ${OpenCV_LIBS}
        protobuf::libprotobuf
        gRPC::grpc++
        Boost::json 
)

gtest_discover_tests(run_tests)