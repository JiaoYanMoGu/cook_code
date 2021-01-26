cmake_minimum_required(VERSION 3.5)

project(corner_edge_feature LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(demo main.cpp)
target_link_libraries(demo ${OpenCV_LIBS})
