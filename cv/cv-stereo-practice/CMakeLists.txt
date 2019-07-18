cmake_minimum_required(VERSION 3.5)
project(opencv_practice)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIR})

add_executable(cv_remap cv_remap.cpp)
target_link_libraries(cv_remap ${OpenCV_LIBS})

add_executable(cv_image_undistort cv_image_undistort.cpp euroc_sequence.h euroc_sequence.cpp)
target_link_libraries(cv_image_undistort ${OpenCV_LIBS})


add_executable(stereo_rectify_map stereo_rectify_map.cpp euroc_sequence.h euroc_sequence.cpp)
target_link_libraries(stereo_rectify_map ${OpenCV_LIBS})



add_executable(stereo_compare stereo_compare.cpp euroc_sequence.h euroc_sequence.cpp)
target_link_libraries(stereo_compare ${OpenCV_LIBS})


add_executable(compute_vio_param compute_vio_param.cpp)
target_link_libraries(compute_vio_param ${OpenCV_LIBS})