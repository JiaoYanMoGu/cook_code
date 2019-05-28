cmake_minimum_required(VERSION 3.0)
project(cv_feature)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(harris_demo harris.cc)
target_link_libraries(harris_demo ${OpenCV_LIBS})
