cmake_minimum_required(VERSION 3.5)

project(LineDetect)

set(OpenCV_DIR /opt/ros/kinetic/share/OpenCV-3.3.1)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lsd_opencv3 lsd_opencv3.cpp)

target_link_libraries(lsd_opencv3 ${OpenCV_LIBS})

