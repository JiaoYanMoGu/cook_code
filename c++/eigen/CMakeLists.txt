cmake_minimum_required(VERSION 3.5)
project(EigenPractice)

find_package(Eigen3 REQUIRED)

include_directories(${Eigen3_INCLUDE_DIRS})
include_directories(/usr/include/eigen3)
add_executable(eigen_practice eigen_practice.cc)

