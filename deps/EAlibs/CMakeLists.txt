# COMMON
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/EASTL/scripts/CMake")
include(CommonCppFlags)

# EABASE
add_library(EABase INTERFACE)
target_include_directories(EABase INTERFACE EABase/include/Common)
target_compile_definitions(EABase INTERFACE -D_CHAR16T)

#EAASSERT
file(GLOB EAASSERT_SOURCES "EAAssert/source/*.cpp")
add_library(EAAssert ${EAASSERT_SOURCES})
target_include_directories(EAAssert PUBLIC EAAssert/include)
target_link_libraries(EAAssert PUBLIC EABase)

#EATHREAD
file(GLOB EATHREAD_SOURCES "EAThread/source/*.cpp")
add_library(EAThread ${EATHREAD_SOURCES})
target_compile_definitions(EAThread PUBLIC -D_CRT_SECURE_NO_WARNINGS)
target_include_directories(EAThread PUBLIC EAThread/include)
target_link_libraries(EAThread PUBLIC EABase)

#EASTDC
file(GLOB EASTDC_SOURCES "EAStdC/source/*.cpp")
add_library(EAStdC ${EASTDC_SOURCES})
target_compile_definitions(EAStdC PUBLIC -D_SCL_SECURE_NO_WARNINGS)
target_include_directories(EAStdC PUBLIC EAStdC/include)
target_link_libraries(EAStdC PUBLIC EABase)
target_link_libraries(EAStdC PUBLIC EAAssert)
target_link_libraries(EAStdC PUBLIC EAThread)

#EASTL

file(GLOB EASTL_SOURCES "EASTL/source/*.cpp")
add_library(EASTL ${EASTL_SOURCES})
target_compile_definitions(EASTL PUBLIC -DEASTL_OPENSOURCE=1)
target_include_directories(EASTL PUBLIC EASTL/include)
target_link_libraries(EASTL PUBLIC EABase)

