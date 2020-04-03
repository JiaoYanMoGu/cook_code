cmake_minimum_required(VERSION 3.5)
project(lesson4)

set(CMAKE_CXX_STANDARD 14)

set(Pangolin_DIR /home/a409/xl_workspace/packages/lib/lib/cmake/Pangolin)

find_package(Pangolin REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/home/a409/xl_workspace/packages/lib/include)
include_directories(${Pangolin_INCLUDE_DIRS})
add_executable(image_undistort cmdline.hpp image_undistort.cpp)
target_link_libraries(image_undistort ${Pangolin_LIBRARIES} ${OpenCV_LIBS})

add_executable(stereo_pointcloud stereo_pointcloud.cpp cmdline.hpp)
target_link_libraries(stereo_pointcloud ${OpenCV_LIBS} ${Pangolin_LIBRARIES})


add_executable(gaussnewton gaussnewton.cpp)
target_link_libraries(gaussnewton ${OpenCV_LIBS})