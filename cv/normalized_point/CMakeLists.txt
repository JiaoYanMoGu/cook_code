cmake_minimum_required(VERSION 3.5)
project(NormalizedPoint)

find_package(OpenCV REQUIRED)

add_definitions(-std=c++11)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(normalized main.cpp)
target_link_libraries(normalized ${OpenCV_LIBS})


