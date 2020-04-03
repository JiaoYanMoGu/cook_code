cmake_minimum_required(VERSION 2.8)

project(icp_align)


set(CMAKE_CXX_STANDARD 11)

find_package(Pangolin REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)


include_directories(${Pangolin_INCLUDE_DIR})
include_directories(${Eigen3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIR})

add_library(Sophus sophus/so3.h sophus/so3.cpp sophus/se3.h sophus/se3.cpp)
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Pangolin_LIBRARIES} Sophus)

