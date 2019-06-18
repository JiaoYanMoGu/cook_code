cmake_minimum_required(VERSION 2.8)

project(harris)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "harris.cpp")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

add_executable(fast fast.cpp)
target_link_libraries(fast ${OpenCV_LIBS})
