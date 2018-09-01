cmake_minimum_required(VERSION 3.5)

project(camera_undis)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(camera_undis main.cpp)

target_link_libraries(camera_undis ${OpenCV_LIBS})

