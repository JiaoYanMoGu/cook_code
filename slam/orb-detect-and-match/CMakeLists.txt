cmake_minimum_required(VERSION 2.8)

project(orb_detect_and_match)

add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} detect_bf_match.cpp  ORBextractor.cc)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
