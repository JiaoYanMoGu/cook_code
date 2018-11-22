cmake_minimum_required(VERSION 3.5)
project(simple_ba)

set(CMAKE_CXX_STANDARD 11)
find_package(Ceres REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${Eigen_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS})


add_executable(simple_ba main.cpp Data.cpp Data.h BALProblem.cpp BALProblem.h)

target_link_libraries(simple_ba ${CERES_LIBRARIES})
