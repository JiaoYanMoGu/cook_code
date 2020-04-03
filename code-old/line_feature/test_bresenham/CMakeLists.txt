cmake_minimum_required(VERSION 3.5)

project(test_bresenham LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(test_bresenham main.cpp line_iterator.cpp)
target_link_libraries(test_bresenham ${OpenCV_LIBS})
