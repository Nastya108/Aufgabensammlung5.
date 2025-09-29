cmake_minimum_required(VERSION 3.15)
project(framework_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLM — заголовочная (header-only) библиотека
find_package(glm QUIET)

# Все исходники фреймворка лежат прямо в этом каталоге (НЕ в src/)
set(FW_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(framework_src STATIC
    ${FW_DIR}/Composite.cpp
    ${FW_DIR}/Composite.hpp
    ${FW_DIR}/renderer.cpp
    ${FW_DIR}/scene_loader.cpp
    ${FW_DIR}/Shape.cpp
    ${FW_DIR}/Sphere.cpp
    ${FW_DIR}/Box.cpp
    ${FW_DIR}/Material.cpp
    ${FW_DIR}/pixel.cpp
    ${FW_DIR}/ppmwriter.cpp
)

# Заголовки доступны потребителям библиотеки
target_include_directories(framework_src PUBLIC ${FW_DIR})

# Если find_package(glm) сработал — линкуемся с импортированной целью
if (glm_FOUND)
  target_link_libraries(framework_src PUBLIC glm::glm)
endif()

# (опционально) красивый алиас
add_library(framework ALIAS framework_src)
