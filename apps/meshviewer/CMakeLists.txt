set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meshviewer)

add_executable(meshviewer meshviewer.cpp)

target_link_libraries(meshviewer PRIVATE RenderComponents)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pbr.vert ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pbr.vert)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pbr.frag ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pbr.frag)
