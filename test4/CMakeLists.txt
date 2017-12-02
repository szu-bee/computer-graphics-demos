cmake_minimum_required(VERSION 3.7)
project(test3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Texture_Mapping.cpp mesh.cpp Mesh_Painter.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ./Common/InitShader.cpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
        ./include
        /usr/include/FreeImage.h
)
target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        /usr/lib/libfreeimage.a
)
