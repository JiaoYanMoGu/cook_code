cmake_minimum_required(VERSION 2.6)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS})


add_executable(app ConsoleApplication1.cpp)

target_link_libraries(app ${OPENGL_LIBRARY} ${ASSIMP_LIBRARIES} glut)

