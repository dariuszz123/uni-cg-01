cmake_minimum_required(VERSION 2.8.4)
project(fractal)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(GLUT)
find_package(OpenGL)

include_directories("C:/MinGW/freeglut/include")
link_directories("C:/MinGW/freeglut/lib")

add_executable(fractal main.cpp)

target_link_libraries(fractal freeglut)
target_link_libraries(fractal glu32)
target_link_libraries(fractal opengl32)