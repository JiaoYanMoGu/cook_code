cmake_minimum_required(VERSION 3.5)

project(ceres_practice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
#add_executable(ceres_practice main.cpp)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world ${CERES_LIBRARIES})

add_executable(numeric_derivatives numeric_derivatives.cpp)
target_link_libraries(numeric_derivatives ${CERES_LIBRARIES})


add_executable(powell_function powell_function.cpp)
target_link_libraries(powell_function ${CERES_LIBRARIES})


add_executable(curve_fitting curve_fitting.cpp)
target_link_libraries(curve_fitting ${CERES_LIBRARIES})


add_executable(robust_curve_fitting robust_curve_fitting.cpp)
target_link_libraries(robust_curve_fitting ${CERES_LIBRARIES})
