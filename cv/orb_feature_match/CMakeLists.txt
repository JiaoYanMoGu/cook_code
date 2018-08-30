cmake_minimum_required(VERSION 3.5)
project(Pracice_project)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE  Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INDLUED_DIRS})
include_directories(./)




add_executable(ORB_match   ORBextractor.cc match_orb.cpp ORBmatcherBF.cpp ORBmatcherBF.h ORBmatcherHF.cpp ORBmatcherHF.h)
target_link_libraries(ORB_match ${OpenCV_LIBS})
