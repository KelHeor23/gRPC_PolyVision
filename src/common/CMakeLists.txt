# Объявляем статическую библиотеку
add_library(common_lib STATIC
    src/impl/OpenCVEncoder.cpp
)

# Указываем директорию с заголовками для самой библиотеки и для тех, кто её подключает
target_include_directories(common_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линкуем библиотеки
target_link_libraries(common_lib
    ${OpenCV_LIBS}
)