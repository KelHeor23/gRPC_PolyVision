# Ищем Google Test
find_package(GTest REQUIRED)

# Создаём исполняемый файл с тестами
add_executable(run_tests 
    client/file_reader_test.cc
    client/json_parser_test.cc
    client/polygon_parser_test.cc
    client/polygons_test.cc
    common/opencv_encoder_test.cc
    server/class_mapper_test.cc
    server/polygon_processor_test.cc
    server/geometric_filter_by_polygon_test.cc
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