cmake_minimum_required(VERSION 3.5)
project(mvg_demo)

add_definitions(-std=c++11)
include_directories(./)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(mvg_demo main.cpp ORBextractor.cc ORBmatcherBF.cpp)

target_link_libraries(mvg_demo ${OpenCV_LIBS})
