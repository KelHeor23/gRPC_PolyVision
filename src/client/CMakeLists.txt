add_library(client_lib STATIC
    src/polygons/Polygons.cpp
    src/polygons/FileReader.cpp
    src/polygons/JsonParser.cpp
    src/polygons/PolygonParser.cpp
    src/processing/ImageProcessingClient.cpp
    src/impl/OpenCVDisplay.cpp
    src/impl/GrpcStreamClient.cpp
    src/impl/CmdOptions.cpp
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

add_executable(client main.cpp)

target_link_libraries(client PRIVATE client_lib)