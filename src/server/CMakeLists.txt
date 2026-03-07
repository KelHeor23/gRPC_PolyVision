add_library(server_lib STATIC
    src/processing/image_processing_server.cc
    src/impl/objects_detecting.cc
    src/impl/yolo_detector.cc
    src/impl/class_mapper.cc
    src/impl/object_filter_by_polygon.cc
    src/impl/drawer.cc
    src/impl/polygon_processor.cc
    src/impl/cmd_options.cc
)

target_include_directories(server_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server_lib PUBLIC
    proto_generated
    common_lib
    ${OpenCV_LIBS}
    Boost::program_options
)

add_executable(server main.cc)

target_link_libraries(server PRIVATE server_lib)