cmake_minimum_required(VERSION 3.1)
project(gflags_practice)

set(CMAKE_CXX_STANDARD 14)

find_package(gflags REQUIRED)

include_directories(${GFLAGS_INCLUDE_DIR})

add_executable(gflags_practice main.cpp config.cpp config.h)
target_link_libraries(gflags_practice ${GFLAGS_LIBRARIES})
