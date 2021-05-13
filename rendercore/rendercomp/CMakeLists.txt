set(RENDERCOMP_CORE_HEADERS
    include/rendercomp/common/FilesystemUtils.h
    include/rendercomp/common/Types.h
    include/rendercomp/core/BoundingBox.h
    include/rendercomp/core/math/Geometry.h
    include/rendercomp/core/math/Intersection.h
    include/rendercomp/core/math/Transform.h
    include/rendercomp/core/AbstractCamera.h
    include/rendercomp/core/cameras/OrthographicCamera.h
    include/rendercomp/core/cameras/PerspectiveCamera.h
    include/rendercomp/core/Resources.h
    include/rendercomp/core/resources/Image.h
    include/rendercomp/core/resources/Mesh.h
    include/rendercomp/core/resources/Volume.h
    include/rendercomp/core/resources/ShaderCode.h
    include/rendercomp/driver/Graphics.h
    include/rendercomp/driver/Mesh.h
    include/rendercomp/driver/Program.h
    include/rendercomp/driver/UniformBuffer.h
    include/rendercomp/driver/Window.h
    include/rendercomp/ui/Widget.h)

set(RENDERCOMP_CORE_SOURCES
    src/common/EASTLNew.cpp
    src/common/FilesystemUtils.cpp
    src/core/BoundingBox.cpp
    src/core/math/Geometry.cpp
    src/core/math/Intersection.cpp
    src/core/math/Transform.cpp
    src/core/AbstractCamera.cpp
    src/core/cameras/OrthographicCamera.cpp
    src/core/cameras/PerspectiveCamera.cpp
    src/core/resources/Mesh.cpp
    src/core/resources/ShaderCode.cpp
    src/driver/Graphics.cpp
    src/driver/Mesh.cpp
    src/driver/Program.cpp
    src/driver/UniformBuffer.cpp
    src/driver/Window.cpp
    src/ui/Widget.cpp)

add_library(RenderComponents STATIC ${RENDERCOMP_CORE_HEADERS} ${RENDERCOMP_CORE_SOURCES})

target_include_directories(RenderComponents PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(RenderComponents PRIVATE assimp)
target_link_libraries(RenderComponents PRIVATE glfw)
target_link_libraries(RenderComponents PUBLIC glm)
target_link_libraries(RenderComponents PUBLIC EASTL)
target_link_libraries(RenderComponents PUBLIC gladloader)
target_link_libraries(RenderComponents PUBLIC imgui)
