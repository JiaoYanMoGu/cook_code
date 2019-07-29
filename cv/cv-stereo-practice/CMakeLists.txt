cmake_minimum_required(VERSION 3.5)
project(opencv_practice)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIR})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


add_library(euroc SHARED euroc_sequence.h euroc_sequence.cpp)

add_executable(cv_remap cv_remap.cpp)
target_link_libraries(cv_remap ${OpenCV_LIBS})

add_executable(cv_image_undistort cv_image_undistort.cpp )
target_link_libraries(cv_image_undistort ${OpenCV_LIBS} euroc)


add_executable(stereo_rectify_map stereo_rectify_map.cpp )
target_link_libraries(stereo_rectify_map ${OpenCV_LIBS} euroc)



add_executable(stereo_compare stereo_compare.cpp )
target_link_libraries(stereo_compare ${OpenCV_LIBS} euroc)


add_executable(compute_vio_param compute_vio_param.cpp)
target_link_libraries(compute_vio_param ${OpenCV_LIBS})

add_executable(verify_euroc_params verify_euroc_params.cpp)
target_link_libraries(verify_euroc_params ${OpenCV_LIBS})

add_executable(view_stereo_image view_stereo_image.cpp cmdline.hpp)
target_link_libraries(view_stereo_image ${OpenCV_LIBS} euroc)