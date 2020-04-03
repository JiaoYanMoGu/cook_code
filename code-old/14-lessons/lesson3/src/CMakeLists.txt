cmake_minimum_required(VERSION 3.5)
project(14_lessons)


set(Pangolin_DIR /home/a409/xl_workspace/packages/lib/lib/cmake/Pangolin)
include_directories(/home/a409/xl_workspace/packages/lib/include)

find_package(Pangolin REQUIRED)
set(CMAKE_CXX_STANDARD 11)
include_directories(${Pangolin_INCLUDE_DIR})
add_executable(draw_traj draw_traj.cpp cmdline.hpp)
target_link_libraries(draw_traj ${Pangolin_LIBRARIES})

add_executable(compute_error compute_error.cpp)


