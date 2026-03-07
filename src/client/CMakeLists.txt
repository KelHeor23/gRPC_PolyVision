add_library(client_lib STATIC
    src/polygons/polygons.cc
    src/polygons/file_reader.cc
    src/polygons/json_parser.cc
    src/polygons/polygon_parser.cc
    src/processing/image_processing_client.cc
    src/impl/opencv_display.cc
    src/impl/grpc_stream_client.cc
    src/impl/cmd_options.cc
)

target_include_directories(client_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(client_lib PUBLIC
    common_lib
    proto_generated
    Boost::json
    Boost::program_options
    ${OpenCV_LIBS}
)

add_executable(client main.cc)

target_link_libraries(client PRIVATE client_lib)