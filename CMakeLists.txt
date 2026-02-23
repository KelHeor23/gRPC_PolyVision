cmake_minimum_required(VERSION 3.14)
project(gRPC_PolyVision)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# Поиск пакетов
# -------------------------------------------------------------------
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

# -------------------------------------------------------------------
# Путь к proto-файлу
# -------------------------------------------------------------------
set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(PROTO_FILE "${PROTO_PATH}/ImageAnalysis.proto")

# -------------------------------------------------------------------
# Поиск плагина gRPC (grpc_cpp_plugin)
# -------------------------------------------------------------------
if(TARGET gRPC::grpc_cpp_plugin)
    get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
else()
    find_program(grpc_cpp_plugin_location NAMES grpc_cpp_plugin)
endif()

if(NOT grpc_cpp_plugin_location)
    message(FATAL_ERROR "grpc_cpp_plugin not found. Install protobuf-compiler-grpc.")
endif()
message(STATUS "gRPC plugin found: ${grpc_cpp_plugin_location}")

# -------------------------------------------------------------------
# Генерация обычных protobuf-файлов (.pb.cc / .pb.h)
# -------------------------------------------------------------------
set(GENERATED_PROTO_SRCS)
protobuf_generate(
    PATH ${PROTO_PATH}
    OUT_VAR GENERATED_PROTO_SRCS
    LANGUAGE cpp
    PROTOS ${PROTO_FILE}
)
# protobuf_generate создаст файлы в ${CMAKE_CURRENT_BINARY_DIR}/proto/

# -------------------------------------------------------------------
# Генерация gRPC-файлов (.grpc.pb.cc / .grpc.pb.h) через add_custom_command
# -------------------------------------------------------------------
set(GRPC_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/proto")
set(GENERATED_GRPC_SRCS "${GRPC_OUT_DIR}/ImageAnalysis.grpc.pb.cc")
set(GENERATED_GRPC_HDRS "${GRPC_OUT_DIR}/ImageAnalysis.grpc.pb.h")

add_custom_command(
    OUTPUT ${GENERATED_GRPC_SRCS} ${GENERATED_GRPC_HDRS}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GRPC_OUT_DIR}
    COMMAND protoc
        --proto_path=${PROTO_PATH}
        --grpc_out=${GRPC_OUT_DIR}
        --plugin=protoc-gen-grpc=${grpc_cpp_plugin_location}
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating gRPC C++ sources"
    VERBATIM
)

# -------------------------------------------------------------------
# Библиотека с общими сгенерированными файлами
# -------------------------------------------------------------------
add_library(proto_generated
    ${GENERATED_PROTO_SRCS}
    ${GENERATED_GRPC_SRCS}
)

target_include_directories(proto_generated PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}  # для доступа к proto/...
)

target_link_libraries(proto_generated
    protobuf::libprotobuf
    gRPC::grpc++
)

# -------------------------------------------------------------------
# Добавляем подпроекты клиента и сервера
# -------------------------------------------------------------------
add_subdirectory(src/client)
add_subdirectory(src/server)