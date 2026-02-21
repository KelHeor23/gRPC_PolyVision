# Исполняемый файл клиента
add_executable(client main.cpp)

# Подключаем директории с заголовками:
# - для proto_generated (библиотека уже добавила include)
# - для общих утилит (если есть common)
#target_include_directories(client PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/../../common   # если нужны общие утилиты
#)

# Линкуем библиотеки
target_link_libraries(client
    proto_generated
    ${OpenCV_LIBS}
)