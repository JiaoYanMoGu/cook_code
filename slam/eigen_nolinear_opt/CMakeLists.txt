cmake_minimum_required(VERSION 3.0)
project(nolinear_opt)
add_definitions(-std=c++11)
find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(gradient_descent gradient_descent.cpp)
