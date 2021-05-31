#include <iostream>
#include <cstring>

#include <rendercomp/common/FilesystemUtils.h>
#include <rendercomp/core/cameras/PerspectiveCamera.h>
#include <rendercomp/core/resources/Mesh.h>
#include <rendercomp/core/resources/ShaderCode.h>
#include <rendercomp/core/resources/Volume.h>
#include <rendercomp/driver/Graphics.h>
#include <rendercomp/driver/Mesh.h>
#include <rendercomp/driver/Program.h>
#include <rendercomp/driver/Texture3D.h>
#include <rendercomp/driver/UniformBuffer.h>
#include <rendercomp/driver/Window.h>

using namespace rendercomp;

data::Mesh CreateCubeMesh() noexcept
{
    data::Mesh cube;
    cube.vertexPositions.reserve(8);
    cube.vertexPositions.emplace_back(-1.f, 1.f, -1.f);
    cube.vertexPositions.emplace_back(-1.f, -1.f, -1.f);
    cube.vertexPositions.emplace_back(1.f, 1.f, -1.f);
    cube.vertexPositions.emplace_back(1.f, -1.f, -1.f);
    cube.vertexPositions.emplace_back(-1.f, 1.f, 1.f);
    cube.vertexPositions.emplace_back(-1.f, -1.f, 1.f);
    cube.vertexPositions.emplace_back(1.f, 1.f, 1.f);
    cube.vertexPositions.emplace_back(1.f, -1.f, 1.f);
    cube.faces.reserve(12);
    cube.faces.emplace_back(0, 1, 3); // front
    cube.faces.emplace_back(0, 3, 2);
    cube.faces.emplace_back(4, 0, 2); // top
    cube.faces.emplace_back(4, 2, 6);
    cube.faces.emplace_back(2, 3, 7); // right
    cube.faces.emplace_back(2, 7, 6);
    cube.faces.emplace_back(6, 7, 5); // back
    cube.faces.emplace_back(6, 5, 4);
    cube.faces.emplace_back(4, 5, 1); // left
    cube.faces.emplace_back(4, 1, 0);
    cube.faces.emplace_back(1, 5, 7); // bottom
    cube.faces.emplace_back(1, 7, 3);

    return cube;
}

data::Volume CreateVolume() noexcept
{
    data::Volume volume;
    volume.channels = 1;     // only 1 channel
    volume.channelDepth = 1; // 1 byte per channel
    volume.width = 128;
    volume.height = 128;
    volume.depth = 128;

    const size_t sliceSize = volume.width * volume.height;
    const size_t totalSize = sliceSize * volume.depth;
    volume.data.resize(totalSize, 0);

    const float frequency = 2.f;

    for(size_t i = 0; i < totalSize; ++i)
    {
        const size_t slice = i % sliceSize;
        const double p = glm::perlin(Vec3f(static_cast<float>(slice / (volume.height))
                                           / static_cast<float>(volume.width),
                                           static_cast<float>(i / (sliceSize))
                                           / static_cast<float>(volume.depth),
                                           static_cast<float>(slice % (volume.width))
                                           / static_cast<float>(volume.height))
                                     * frequency);

        volume.data[i] = p > 0.0? static_cast<uint8_t>(255.0 * p) : 0;
    }

    return volume;
}

int main(int, char** argv)
{
    // Initialize filesystem to work on executable directory
    FilesystemUtils::init(argv[0]);
    const String basePath = FilesystemUtils::currentPath();

    WindowConfiguration wConfig;
    wConfig.title = "Volume raymarcher";
    wConfig.width = 1536;
    wConfig.height = 864;
    Window window(wConfig);

    // Perspective camera on 0,0,-5
    PerspectiveCamera camera (0.1f,
                              100.f,
                              static_cast<float>(1536) / static_cast<float>(864),
                              45.f);
    camera.translate({0.f, 0.f, 5.f});
    camera.updateView();
    // Camera buffer (projView matrix + cam position)
    UniformBuffer camBuffer (sizeof(float) * (16 + 3),
                             BufferDataPolicy::DYNAMIC,
                             BufferUsagePolicy::DRAW);

    auto camUpdateCb = [camPtr = &camera](char* data, const size_t)
    {
        memcpy(data, &camPtr->getProjectionMatrix()[0][0], sizeof(float) * 16);
        size_t offset = sizeof(float) * 16;

        memcpy(data + offset, &camPtr->getViewMatrix()[0][0], sizeof(float) * 16);
        offset += sizeof(float) * 16;

        const auto camPos = camPtr->getPosition();
        memcpy(data + offset, &camPos[0], sizeof(float) * 3);
        offset += sizeof(float) * 4;

        memcpy(data + offset, &camPtr->forward()[0], sizeof(float) * 3);
    };

    // Initialize camera buffer
    camBuffer.writeData(camUpdateCb);
    constexpr uint32_t camBufferBinding = 0;
    camBuffer.bindToPoint(camBufferBinding);

    // Add controls to rotate camera
    struct CameraControlParams
    {
        bool leftDown {false};
        bool resetRot {false};
        double lastRotMouseX = -1.0;
        double lastRotMouseY = -1.0;
        float rotY {0.f};
        float rotX {0.f};
    };
    CameraControlParams ccp;
    window.setMouseInputHandler([ccpPtr = &ccp](const MouseButton button,
                                                const MouseButtonAction action,
                                                const InputMod)
    {
        if(button == MouseButton::MOUSE_BUTTON_LEFT)
        {
            ccpPtr->leftDown = action == MouseButtonAction::ACTION_PRESS;
            ccpPtr->resetRot = ccpPtr->leftDown;
        }
    });
    window.setCursorPositionHandler([ccpPtr = &ccp,
                                     camPtr = &camera,
                                     camBufferPtr = &camBuffer,
                                     camUpdateCbPtr = &camUpdateCb]
    (const double x, const double y)
    {
        if(ccpPtr->resetRot)
        {
            ccpPtr->lastRotMouseX = x;
            ccpPtr->lastRotMouseY = y;
            ccpPtr->resetRot = false;
            return;
        }
        if(ccpPtr->leftDown)
        {
            const double deltaX = x - ccpPtr->lastRotMouseX;
            ccpPtr->lastRotMouseX = x;
            ccpPtr->rotY += deltaX;
            const double deltaY = y - ccpPtr->lastRotMouseY;
            ccpPtr->lastRotMouseY = y;
            ccpPtr->rotX += deltaY;

            camPtr->setRotation(Quaternion({glm::radians(ccpPtr->rotX), 0.f, 0.f}) *
                                Quaternion({0.f, glm::radians(ccpPtr->rotY), 0.f}));
            camPtr->updateView();
            camBufferPtr->writeData(*camUpdateCbPtr);
        }
    });

    // Create a cube which we will rasterize to raymarch the volume
    data::Mesh cube = CreateCubeMesh();
    const Mesh mesh (&cube);
    mesh.bind();

    const data::Volume noise = CreateVolume();
    const Texture3D volume (&noise, 6);
    volume.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR);
    volume.setMagFilter(TextureMagFilter::LINEAR);
    volume.setWrapR(TextureWrapMode::CLAMP_TO_EDGE);
    volume.setWrapS(TextureWrapMode::CLAMP_TO_EDGE);
    volume.setWrapT(TextureWrapMode::CLAMP_TO_EDGE);
    volume.bind(0);

    auto vShader = Resources::load<data::ShaderCode>(basePath + "/raymarch.vert");
    auto fShader = Resources::load<data::ShaderCode>(basePath + "/raymarch.frag");
    Program shader (vShader->getRawCode(), fShader->getRawCode());
    shader.use();
    shader.setUniformBlockBinding("Camera", camBufferBinding);
    const auto minCornerU = shader.getUniform("minB");
    const auto maxCornerU = shader.getUniform("maxB");
    shader.setUniform(minCornerU, Vec3f(-1.f, -1.f, -1.f));
    shader.setUniform(maxCornerU, Vec3f(1.f, 1.f, 1.f));

    Graphics::enable(Feature::DEPTH_TEST);
    Graphics::enable(Feature::BLEND);
    Graphics::setBlendFunc(BlendFunc::ONE, BlendFunc::ONE_MINUS_SRC_ALPHA);

    window.setDrawCallback([meshPtr = &mesh]()
    {
        Graphics::clearBuffers(BufferFlag::COLOR | BufferFlag::DEPTH);
        Graphics::drawElement(Primitive::TRIANGLES, meshPtr->getNumDrawElements());
    });
    window.renderLoop();

    return 0;
}
