
# ================================================================================
# ASSIMP
# ================================================================================

SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL
    "If the supplementary tools for Assimp are built in addition to the library.")
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL
    "If the test suite for Assimp is built in addition to the library.")
SET(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL
    "If the Assimp view tool is built. (requires DirectX)")

add_subdirectory(assimp)

# ================================================================================
# EA Libraries
# ================================================================================

add_subdirectory(EAlibs)

# ================================================================================
# GLAD
# ================================================================================

add_subdirectory(glad)

# ================================================================================
# GLFW
# ================================================================================

SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programss")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(glfw)

# ================================================================================
# GLM
# ================================================================================

SET(GLM_TEST_ENABLE_CXX_14 ON CACHE BOOL "Enable C++ 14")

add_subdirectory(glm)

# ================================================================================
# IMGUI
# ================================================================================

SET(IMGUI_HEADERS
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h)

SET(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp)

add_library(imgui ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui PUBLIC imgui/backends)
target_link_libraries(imgui PUBLIC gladloader)
target_link_libraries(imgui PUBLIC glfw)
