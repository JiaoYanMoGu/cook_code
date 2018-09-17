project(simple_plugin_lib)
cmake_minimum_required(VERSION 3.5)

add_definitions(-std=c++11)

include_directories(./)
add_executable(plugin_test main.cpp)

