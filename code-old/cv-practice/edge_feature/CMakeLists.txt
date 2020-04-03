cmake_minimum_required(VERSION 3.5)

project(edge_feature LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lsd_demo line_detect.cpp)
target_link_libraries(lsd_demo ${OpenCV_LIBS})


add_executable(edge_point_detect edge_point_detect.cpp)
target_link_libraries(edge_point_detect ${OpenCV_LIBS})
