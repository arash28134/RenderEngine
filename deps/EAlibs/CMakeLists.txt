# COMMON
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/EASTL/scripts/CMake")
include(CommonCppFlags)

# EABASE
add_library(EABase INTERFACE)
target_include_directories(EABase INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/EABase/include/Common)
target_compile_definitions(EABase INTERFACE -D_CHAR16T)

#EASTL
file(GLOB EASTL_SOURCES "EASTL/source/*.cpp")
add_library(EASTL STATIC ${EASTL_SOURCES})

target_compile_definitions(EASTL PUBLIC -D_CHAR16T)
target_compile_definitions(EASTL PUBLIC -D_CRT_SECURE_NO_WARNINGS)
target_compile_definitions(EASTL PUBLIC -D_SCL_SECURE_NO_WARNINGS)
target_compile_definitions(EASTL PUBLIC -DEASTL_OPENSOURCE=1)

target_include_directories(EASTL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/EASTL/include)
target_link_libraries(EASTL PUBLIC EABase)
