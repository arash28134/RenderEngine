cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake")

find_package(OpenGL REQUIRED)
if(UNIX)
    find_package(FreeImage REQUIRED)
else()

endif()

add_subdirectory(deps)
