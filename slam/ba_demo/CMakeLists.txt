cmake_minimum_required(VERSION 3.5)
project(simple_ba)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)


find_package(Ceres REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Pangolin REQUIRED)




include_directories(${Eigen_INCLUDE_DIRS})
include_directories(${CERES_INCLUDE_DIRS})
include_directories(${Pangolin_INCLUDE_DIRS})


add_executable(simple_ba src/main.cpp src/Data.cpp src/Data.h src/BALProblem.cpp src/BALProblem.h src/viewer.h src/viewer.cpp src/common.hpp)
target_link_libraries(simple_ba ${CERES_LIBRARIES} ${Pangolin_LIBRARIES} pthread)

