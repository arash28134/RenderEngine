set(C_GLAD_SOURCES src/glad.c)

set(C_GLAD_HEADERS
  include/glad/glad.h
  include/KHR/khrplatform.h
)

add_library(gladloader STATIC ${C_GLAD_HEADERS} ${C_GLAD_SOURCES})

target_include_directories(gladloader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
