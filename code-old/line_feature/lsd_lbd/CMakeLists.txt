cmake_minimum_required(VERSION 3.5)

project(lsd_lbd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

set(OpenCV_DIR /usr/share/OpenCV)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS}
                    /usr/include/eigen3
                    /opt/Sophus/include)


add_subdirectory(dataset_io)

add_executable(canny_edge canny_edge.cpp)
target_link_libraries(canny_edge ${OpenCV_LIBS})

add_executable(lsd_detect lsd_detect.cpp)
target_link_libraries(lsd_detect ${OpenCV_LIBS})

add_executable(hough_line hough_line.cpp)
target_link_libraries(hough_line ${OpenCV_LIBS})

add_executable(fast_line_detector fast_line.cpp)
target_link_libraries(fast_line_detector ${OpenCV_LIBS} dataset_io)

add_executable(lsd_lbd lsd_lbd.cpp)
target_link_libraries(lsd_lbd ${OpenCV_LIBS} dataset_io)
