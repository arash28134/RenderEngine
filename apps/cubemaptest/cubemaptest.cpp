#include <iostream>

#include <rendercomp/core/cameras/PerspectiveCamera.h>
#include <rendercomp/core/resources/Image.h>
#include <rendercomp/core/resources/Mesh.h>
#include <rendercomp/core/resources/ShaderCode.h>

#include <rendercomp/driver/Graphics.h>
#include <rendercomp/driver/Mesh.h>
#include <rendercomp/driver/Program.h>
#include <rendercomp/driver/TextureCubemap.h>
#include <rendercomp/driver/UniformBuffer.h>
#include <rendercomp/driver/Window.h>

const uint32_t W_WIDTH = 1536;
const uint32_t W_HEIGHT = 864;

rendercomp::data::Mesh CreateCubeMesh() noexcept
{
    rendercomp::data::Mesh cube;
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

int main(int, char** argv)
{
    using namespace rendercomp;

    // Init filesystem so we track current directory
    FilesystemUtils::init(argv[0]);
    const String basePath = FilesystemUtils::currentPath() + "/";

    // Load images from disk
    auto right  = Resources::load<data::Image>(basePath + "skybox/right.jpg");
    auto left   = Resources::load<data::Image>(basePath + "skybox/left.jpg");
    auto back   = Resources::load<data::Image>(basePath + "skybox/back.jpg");
    auto bottom = Resources::load<data::Image>(basePath + "skybox/bottom.jpg");
    auto front  = Resources::load<data::Image>(basePath + "skybox/front.jpg");
    auto top    = Resources::load<data::Image>(basePath + "skybox/top.jpg");

    // Create Window
    WindowConfiguration config;
    config.title = "Cubemap viewer";
    config.width = W_WIDTH;
    config.height = W_HEIGHT;
    Window window (config);

    // Create a camera
    PerspectiveCamera camera (0.1f,
                              50.f,
                              static_cast<float>(W_WIDTH) / static_cast<float>(W_HEIGHT),
                              45.f);
    // Create a buffer for the camera and initialize it
    UniformBuffer cameraBuffer (sizeof(float) * 16,
                                BufferDataPolicy::DYNAMIC,
                                BufferUsagePolicy::DRAW);
    cameraBuffer.bindToPoint(0);
    const auto updateCameraCb = [camPtr = &camera](char* data, const size_t)
    {
        const auto projView = camPtr->getProjectionMatrix() * camPtr->getViewMatrix();
        memcpy(data, &projView[0][0], sizeof(float) * 16);
    };
    cameraBuffer.writeData(updateCameraCb);

    // Add controls to rotate camera
    struct CameraControlParams
    {
        bool leftDown {false};
        bool rightDown {false};
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
        else if(button == MouseButton::MOUSE_BUTTON_RIGHT)
        {
            ccpPtr->rightDown = action == MouseButtonAction::ACTION_PRESS;
            ccpPtr->resetRot = ccpPtr->rightDown;
        }
    });
    window.setCursorPositionHandler([ccpPtr = &ccp,
                                     camPtr = &camera,
                                     camBufferPtr = &cameraBuffer,
                                     camUpdateCbPtr = &updateCameraCb]
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

            camPtr->setRotation(Quaternion({glm::radians(ccpPtr->rotX), 0.f, 0.f}) *
                                Quaternion({0.f, glm::radians(ccpPtr->rotY), 0.f}));
            camPtr->updateView();
            camBufferPtr->writeData(*camUpdateCbPtr);
        }
        else if(ccpPtr->rightDown)
        {
            const double deltaY = y - ccpPtr->lastRotMouseY;
            ccpPtr->lastRotMouseY = y;
            ccpPtr->rotX += deltaY;

            camPtr->setRotation(Quaternion({glm::radians(ccpPtr->rotX), 0.f, 0.f}) *
                                Quaternion({0.f, glm::radians(ccpPtr->rotY), 0.f}));
            camPtr->updateView();
            camBufferPtr->writeData(*camUpdateCbPtr);
        }
    });

    // Create an unit cube
    data::Mesh cube = CreateCubeMesh();
    const Mesh mesh(&cube);
    mesh.bind();

    // Create the shader
    auto vshader = Resources::load<data::ShaderCode>(basePath + "cubemap.vert");
    auto fshader = Resources::load<data::ShaderCode>(basePath + "cubemap.frag");
    const Program shader (vshader->getRawCode(), fshader->getRawCode());
    shader.use();
    shader.setUniformBlockBinding("Camera", 0);

    // Create the cubemap texture
    CubemapImages faces;
    faces.right = right.get();
    faces.left = left.get();
    faces.back = back.get();
    faces.bottom = bottom.get();
    faces.front = front.get();
    faces.top = top.get();
    const TextureCubemap cubemap (faces);
    cubemap.setWrapS(TextureWrapMode::CLAMP_TO_EDGE);
    cubemap.setWrapT(TextureWrapMode::CLAMP_TO_EDGE);
    cubemap.setWrapR(TextureWrapMode::CLAMP_TO_EDGE);
    cubemap.setMinFilter(TextureMinFilter::LINEAR);
    cubemap.setMagFilter(TextureMagFilter::LINEAR);
    cubemap.bind(0);

    Graphics::disable(Feature::DEPTH_TEST);
    Graphics::disable(Feature::CULL_FACE);

    // Render
    window.setDrawCallback([meshPtr = &mesh]
    {
        Graphics::clearBuffers(BufferFlag::COLOR);
        Graphics::drawElement(Primitive::TRIANGLES, meshPtr->getNumDrawElements());
    });
    window.renderLoop();

    return 0;
}
