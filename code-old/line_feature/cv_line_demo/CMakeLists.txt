cmake_minimum_required(VERSION 3.5)

project(cv_line_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR /usr/share/OpenCV)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS}
                    /usr/include/eigen3
                    /opt/Sophus/include)
add_subdirectory(dataset_io)
add_executable(line_match line_match.cpp)
target_link_libraries(line_match ${OpenCV_LIBS} dataset_io)

add_executable(line_extraction line_extraction.cpp)
target_link_libraries(line_extraction ${OpenCV_LIBS} dataset_io)

add_executable(line_tracking line_tracking.cpp line_iterator.cpp)
target_link_libraries(line_tracking ${OpenCV_LIBS} dataset_io)

