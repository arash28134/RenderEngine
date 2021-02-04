cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

add_subdirectory(deps)
