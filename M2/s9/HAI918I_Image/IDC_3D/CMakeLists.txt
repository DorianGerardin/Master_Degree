cmake_minimum_required(VERSION 3.10)
project(hmin322-tp-idc-3D)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PYTHON            "Use Python"         OFF)
#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#add_subdirectory(libigl)

find_package(LIBIGL REQUIRED QUIET)

# Add your project files
file(GLOB SRCFILES tp/*.h tp/*.cpp)
add_executable(tp_bin ${SRCFILES})
target_link_libraries(tp_bin igl::core igl::opengl_glfw)
