cmake_minimum_required(VERSION 3.0)
find_package(Eigen3 REQUIRED)

include_directories(${Eigen3_INCLUDE_DIRS})

add_executable(demo main.cpp)


