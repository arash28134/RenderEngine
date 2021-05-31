set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/volumetest)

add_executable(volumetest volumetest.cpp)

target_link_libraries(volumetest PRIVATE RenderComponents)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/raymarch.vert ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/raymarch.vert)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/raymarch.frag ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/raymarch.frag)
