cmake_minimum_required(VERSION 3.5)

project(cv_optical_flow LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})


find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRS})

add_executable(cv_optical_flow main.cpp optical_flow.h optical_flow.cpp )
target_link_libraries(cv_optical_flow ${OpenCV_LIBS})

add_executable(direct_method direct_method.cpp util.h compute_pose.h compute_pose.cpp pyramid_builder.h pyramid_builder.cpp)
target_link_libraries(direct_method ${OpenCV_LIBS} ${Boost_LIBRARIES} )


add_executable(compute_disparity_klt compute_disparity_klt.cpp )
target_link_libraries(compute_disparity_klt ${OpenCV_LIBS} ${Boost_LIBRARIES})



# simple test function programs
add_executable(test_cv_img_pyramid test_cv_img_pyramid.cpp )
target_link_libraries(test_cv_img_pyramid ${OpenCV_LIBS})


add_executable(test_cv_mat_step test_cv_mat_step.cpp)
target_link_libraries(test_cv_mat_step ${OpenCV_LIBS})


add_executable(test_cv_copy test_cv_copy.cpp)
target_link_libraries(test_cv_copy ${OpenCV_LIBS})
