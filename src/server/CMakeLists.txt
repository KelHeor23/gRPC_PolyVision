add_library(server_lib STATIC
    src/processing/ImageProcessingServer.cpp
    src/impl/ObjectsDetecting.cpp
    src/impl/YoloDetector.cpp
    src/impl/ClassMapper.cpp
    src/impl/ObjectFilterByPolygon.cpp
    src/impl/Drawer.cpp
    src/impl/PolygonProcessor.cpp
    src/impl/CmdOptions.cpp
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

add_executable(server main.cpp)

target_link_libraries(server PRIVATE server_lib)