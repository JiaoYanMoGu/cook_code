cmake_minimum_required(VERSION 3.5)
project(vio_dataset)

set(CMAKE_BUILD_TYPE  Release)
set(CMAKE_CXX_STANDARD 11)

#set(Eigen3_DIR /home/a409/xl_workspace/packages/lib/share/eigen3/cmake)
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(vio_dataset main.cpp zju_sequence.cpp zju_sequence.h euroc_sequence.cpp euroc_sequence.h vio_sequence.cpp vio_sequence.h)
target_link_libraries(vio_dataset ${OpenCV_LIBS})
