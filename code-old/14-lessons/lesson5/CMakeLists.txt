cmake_minimum_required(VERSION 3.5)
project(slam_lesson)


add_definitions(-std=c++11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})
include_directories(/home/a409/xl_workspace/packages/lib/include)

include_directories(/home/a409/xl_workspace/packages/lib/include/eigen3)
add_executable(compute_orientation compute_orientation.cpp)
target_link_libraries(compute_orientation ${OpenCV_LIBS})

add_executable(decomposeE decomposeE.cpp)
target_link_libraries(decomposeE ${OpenCV_LIBS})

add_executable(gn_ba gn_ba.cpp)
target_link_libraries(gn_ba ${OpenCV_LIBS))
