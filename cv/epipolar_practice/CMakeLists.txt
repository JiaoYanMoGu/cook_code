cmake_minimum_required(VERSION 3.5)
project(epipolar_practice)

add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)

include_directories(./)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(epipolar_practice   main.cpp  fundamental.cc homography.cc epipolar_practice.pro.user  fundamental.hpp  homography.hpp  ORBextractor.cc)

target_link_libraries(epipolar_practice ${OpenCV_LIBS})
