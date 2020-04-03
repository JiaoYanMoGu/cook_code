cmake_minimum_required(VERSION 3.5)

project(subpix_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})

add_executable(subpix_demo main.cpp)
target_link_libraries(subpix_demo ${OpenCV_LIBS})
