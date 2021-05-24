set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cubemaptest)

add_executable(cubemaptest cubemaptest.cpp)

target_link_libraries(cubemaptest PRIVATE RenderComponents)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cubemap.vert ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cubemap.vert)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cubemap.frag ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cubemap.frag)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/skybox DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
