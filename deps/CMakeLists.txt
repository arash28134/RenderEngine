
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
# FREEIMAGE
# ================================================================================

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
