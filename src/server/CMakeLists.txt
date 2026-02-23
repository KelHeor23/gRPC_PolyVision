add_executable(server main.cpp
    src/processing/ImageProcessingServer.cpp
    src/impl/Drawer.cpp
    src/impl/PolygonProcessor.cpp)

target_include_directories(server PRIVATE include)

target_link_libraries(server
    proto_generated
    common_lib
    ${OpenCV_LIBS}
)