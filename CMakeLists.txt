cmake_minimum_required(VERSION 3.0)
project(opengl_practice VERSION 1.0 LANGUAGES C CXX)

set(MAIN ./src/main.cpp)
set(GLAD ./include/glad.c)
set(SHADER ./src/shader.cpp)
set(CAMERA ./src/camera.cpp)

add_executable(demo ${MAIN} ${GLAD} ${SHADER} ${CAMERA})

target_link_libraries(demo glfw dl)
