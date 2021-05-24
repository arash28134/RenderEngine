set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texturetest)

add_executable(texturetest texturetest.cpp)

target_link_libraries(texturetest PRIVATE RenderComponents)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/texture.vert ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texture.vert)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/texture.frag ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texture.frag)
