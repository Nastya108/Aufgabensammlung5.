cmake_minimum_required(VERSION 3.15)
project(raytracer_app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Здесь лежит main()
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(raytracer
    ${APP_DIR}/raytracer.cpp
    # Если захотите собрать пример отдельно — раскомментируйте:
    # ${APP_DIR}/example.cpp
)

target_include_directories(raytracer PRIVATE ${APP_DIR})

# Линкуем с библиотекой из framework
target_link_libraries(raytracer PRIVATE framework_src)
# или: target_link_libraries(raytracer PRIVATE framework)  # если используете ALIAS
