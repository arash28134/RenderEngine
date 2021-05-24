cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# Append the path to the custom CMake files
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake")

find_package(OpenGL REQUIRED)
if(WIN32)
    include(WindowsPlatform)
    get_windows_platform(PLATFORM_OUT)
    set(FREEIMAGE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/deps/FreeImageDLL/${PLATFORM_OUT})
endif()
find_package(FreeImage REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(deps)

add_subdirectory(rendercore)

add_subdirectory(apps)
