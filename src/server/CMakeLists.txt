add_executable(server main.cpp)

#target_include_directories(server PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
#)

target_link_libraries(server
    proto_generated
    ${OpenCV_LIBS}
)