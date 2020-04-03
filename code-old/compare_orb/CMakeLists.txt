cmake_minimum_required(VERSION 3.5)
project(CompareORB)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE  Release)

set(OpenCV_DIR /opt/ros/kinetic/share/OpenCV-3.3.1-dev)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})


add_executable(CompareORB main_orb_slam2.cpp ORBextractor.h ORBextractor.cpp euroc_sequence.h euroc_sequence.cpp)

target_link_libraries(CompareORB ${OpenCV_LIBS})

add_executable(ice_ba_orb main_ice_ba_orb.cpp cmdline.hpp euroc_sequence.h euroc_sequence.cpp
             ice_ba_orb.h ice_ba_orb.cc)

target_link_libraries(ice_ba_orb ${OpenCV_LIBS} ${GLOG_LIBS} glog)




# SSE_ORB
set(USE_SSE_ORB ON)

if(USE_SSE_ORB)
    message(STATUS "SSE3 for ORB extraction (-msse3): ENABLED")
    add_definitions(-DUSE_SSE_ORB)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
else()
    message(STATUS "SSE3 for ORB extraction (-msse3): DISABLED")
    remove_definitions(-DUSE_SSE_ORB)
endif()

add_executable(openvslam_orb openvslam_orb.cpp euroc_sequence.cpp)

find_package(YamlCpp REQUIRED)

add_subdirectory(feature)

target_link_libraries(openvslam_orb ${OpenCV_LIBS} ${YAMLCPP_LIBRARIES})

