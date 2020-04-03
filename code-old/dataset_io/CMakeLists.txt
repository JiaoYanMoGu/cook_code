cmake_minimum_required(VERSION 3.5)

project(dataset_io LANGUAGES CXX)


#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
set(Sophus_INCLUDE_DIRS /opt/Sophus/include)
include_directories(${OpenCV_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${Sophus_INCLUDE_DIRS})
add_executable(dataset_io main.cpp dataset_io.h dataset_io.cpp
    timestamp.h timestamp.cpp split.h split.cpp
    dataset_euroc.h dataset_euroc.cpp
    dataset_fpv.h dataset_fpv.cpp
    dataset_zju.h dataset_zju.cpp)
target_link_libraries(dataset_io ${OpenCV_LIBS} stdc++fs )
