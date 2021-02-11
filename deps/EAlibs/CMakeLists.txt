# COMMON
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/EASTL/scripts/CMake")
include(CommonCppFlags)

# EABASE
add_library(EABase INTERFACE)
target_include_directories(EABase INTERFACE EABase/include/Common)
target_compile_definitions(EABase INTERFACE -D_CHAR16T)

#EASTL

file(GLOB EASTL_SOURCES "EASTL/source/*.cpp")
add_library(EASTL ${EASTL_SOURCES})
target_compile_definitions(EASTL PUBLIC -DEASTL_OPENSOURCE=1)
target_include_directories(EASTL PUBLIC EASTL/include)
target_link_libraries(EASTL PUBLIC EABase)

