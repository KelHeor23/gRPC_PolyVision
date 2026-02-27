add_executable(server main.cpp
    src/processing/ImageProcessingServer.cpp
    src/impl/ObjectsDetecting.cpp
    src/impl/YoloDetector.cpp
    src/impl/ClassMapper.cpp
    src/impl/ObjectFilterByPolygon.cpp
    src/impl/Drawer.cpp
    src/impl/PolygonProcessor.cpp
    src/impl/CmdOptions.cpp)

target_include_directories(server PRIVATE include)
find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(server
    proto_generated
    common_lib
    ${OpenCV_LIBS}
    Boost::program_options
)