#include <iostream>

#include <rendercomp/common/Types.h>

#include <rendercomp/core/BoundingBox.h>
#include <rendercomp/core/cameras/OrthographicCamera.h>
#include <rendercomp/core/cameras/PerspectiveCamera.h>
#include <rendercomp/core/math/Geometry.h>
#include <rendercomp/core/Resources.h>
#include <rendercomp/core/resources/Mesh.h>
#include <rendercomp/core/resources/ShaderCode.h>

#include <rendercomp/driver/Graphics.h>
#include <rendercomp/driver/UniformBuffer.h>
#include <rendercomp/driver/Mesh.h>
#include <rendercomp/driver/Program.h>
#include <rendercomp/driver/Window.h>

#include <rendercomp/ui/Widget.h>

using namespace rendercomp;

const uint32_t WIDTH = 1536;
const uint32_t HEIGHT = 864;


class ManipulatorWidget : public Widget
{
public:
    ManipulatorWidget(UniformBuffer* materialGpuBuffer)
        : Widget("Manipulator", {WIDTH - 400, 0}, {400, -1})
        , _materialBuffer(materialGpuBuffer)
    {
        _updateMaterialCb = [&](char* ptr, const size_t)
        {
            size_t offset = 0;
            memcpy(ptr, &_albedo[0], sizeof(float) * 3);
            offset += sizeof(float) * 3;
            memcpy(ptr + offset, &_roughness, sizeof(float));
            offset += sizeof(float);
            memcpy(ptr + offset, &_f0[0], sizeof(float) * 3);
            offset += sizeof(float) * 3;
            memcpy(ptr + offset, &_metallic, sizeof(float));
        };
        _materialBuffer->writeData(_updateMaterialCb);
    }

    void drawImpl() const noexcept final
    {
        bool matlUpdated = false;
        matlUpdated = ImGui::ColorEdit3("Albedo", &_albedo[0]);
        matlUpdated = ImGui::SliderFloat("Roughness", &_roughness, 0.01f, 1.f) || matlUpdated;
        matlUpdated = ImGui::SliderFloat("Metallic", &_metallic, 0.01f, 1.f) || matlUpdated;
        matlUpdated = ImGui::SliderFloat3("Base reflectivity", &_f0[0], 0.f, 1.f) || matlUpdated;
        if(matlUpdated)
            _materialBuffer->writeData(_updateMaterialCb);

    }

private:
    mutable std::function<void(char*, const size_t)> _updateMaterialCb;
    mutable UniformBuffer* _materialBuffer;
    mutable Vec3f _albedo {1.f, 0.f, 0.f};
    mutable float _roughness {0.01f};
    mutable float _metallic {0.f};
    mutable Vec3f _f0 {0.04f, 0.04f, 0.04f};
};

int main(int argc, char** args)
{
    if(argc < 2)
    {
        std::cout << "Usage: " << args[0] << " <path to mesh file>" << std::endl;
        return 0;
    }

    FilesystemUtils::init(args[0]);

    // Load the mesh from disk
    const String meshPath(args[1]);
    std::unique_ptr<data::Mesh> meshFile = Resources::load<data::Mesh>(meshPath);
    if(meshFile->vertexNormals.empty())
        meshFile->vertexNormals = ComputeSmoothNormals(meshFile->faces, meshFile->vertexPositions);

    // Create a bounding box of the mesh so we know where to place the camera easily
    AxisAlignedBoundingBox aabb;
    for(const auto& vPos : meshFile->vertexPositions)
        aabb.expand(vPos);

    // Create a window, initializing the graphics context
    WindowConfiguration config;
    config.clearColor = Vec4f(.6f, .6f, .6f, 1.f);
    config.title = String("Mesh viewer");
    config.width = WIDTH;
    config.height = HEIGHT;
    config.resizable = false;
    config.maxFPS = FPS::FPS_60;
    Window window (config);

    // Create a camera
    const float nearPlane = 0.1f;
    const float farPlane = 500.f;
    const float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
    const float fovy = 45.f;
    PerspectiveCamera camera (nearPlane, farPlane, aspectRatio, fovy);

    // Adjust the camera in a position where it can capture the whole mesh
    const float radiansFovy = glm::radians(fovy * 0.5f);
    const float distToModel = (aabb.getYLength() * 0.5f) / sin(radiansFovy) * cos(radiansFovy);
    Vec3f camPos = aabb.center();
    camPos.z = -distToModel * 1.1f;
    camera.transform().setTranslation(camPos);
    camera.updateView();

    // Create a buffer to send the camera info to the GPU
    // (1 mat4 proj view and 1 vec4 for the cam position)
    const size_t uboBindingPoint {0};
    UniformBuffer cameraBuffer (20 * sizeof(float),
                                BufferDataPolicy::DYNAMIC,
                                BufferUsagePolicy::DRAW);
    cameraBuffer.bindToPoint(uboBindingPoint);
    const auto updateCameraBufferCb = [camPtr = &camera](char* ptr, const size_t)
    {
        const Mat4 projView = camPtr->getProjectionMatrix() * camPtr->getViewMatrix();
        memcpy(ptr, &projView[0][0], 16 * sizeof(float));

        const Vec3f camPos = camPtr->transform().worldPosition();
        memcpy(ptr + 16 * sizeof(float), &camPos[0], 3 * sizeof(float));
    };
    cameraBuffer.writeData(updateCameraBufferCb);

    // Create a buffer to send the material information
    const size_t matUboBindingPoint {1};
    UniformBuffer materialBuffer (sizeof(float) * 8,
                                  BufferDataPolicy::DYNAMIC,
                                  BufferUsagePolicy::DRAW);
    materialBuffer.bindToPoint(matUboBindingPoint);
    // Add a widget to handle it
    window.createWidget<ManipulatorWidget>("test_widget", &materialBuffer);

    // Set the mouse input processor callbacks
    bool leftDown = false;
    bool rightDown = false;
    bool resetRot = true;
    double lastRotMouseX = 0.0;
    bool resetPan = true;
    double lastPanMouseX = 0.0;
    double lastPanMouseY = 0.0;
    window.setMouseInputHandler([&](const MouseButton button,
                                    const MouseButtonAction action,
                                    const InputMod)
    {
        if(button == MouseButton::MOUSE_BUTTON_LEFT)
        {
            leftDown = action == MouseButtonAction::ACTION_PRESS;
            resetRot = leftDown;
        }
        else if(button == MouseButton::MOUSE_BUTTON_RIGHT)
        {
            rightDown = action == MouseButtonAction::ACTION_PRESS;
            resetPan = rightDown;
        }
    });
    window.setCursorPositionHandler([&](const double x, const double y)
    {
        if(leftDown)
        {
            if(resetRot)
            {
                lastRotMouseX = x;
                resetRot = false;
                return;
            }

            const double deltaX = x - lastRotMouseX;
            lastRotMouseX = x;

            camera.transform().rotateY(deltaX);
            camera.updateView();
            cameraBuffer.writeData(updateCameraBufferCb);
        }
        else if(rightDown)
        {
            if(resetPan)
            {
                lastPanMouseX = x;
                lastPanMouseY = y;
                resetPan = false;
                return;
            }

            const double deltaX = x - lastPanMouseX;
            const double deltaY = y - lastPanMouseY;
            lastPanMouseX = x;
            lastPanMouseY = y;

            camera.transform().translate({deltaX * 0.01f, deltaY * 0.01f, 0.0f});
            camera.updateView();
            cameraBuffer.writeData(updateCameraBufferCb);
        }
    });
    float modFov = fovy;
    window.setScrollInputHandler([&](const double yDelta)
    {
        modFov -= yDelta;
        modFov = std::max(1.f, std::min(60.f, modFov));
        camera.setFovy(modFov);
        camera.updateProjection();
        cameraBuffer.writeData(updateCameraBufferCb);
    });

    // Upload mesh to GPU and bind it
    // (we will not bind any other mesh, so we do not need to change it during the loop)
    Mesh engMesh (meshFile.get());
    engMesh.bind();

    // Create the program to shade the mesh, and bind it since it wont change
    auto vertexShaderCode = Resources::load<data::ShaderCode>(
                FilesystemUtils::currentPath() + "/pbr.vert");
    auto fragmentShaderCode = Resources::load<data::ShaderCode>(
                FilesystemUtils::currentPath() + "/pbr.frag");
    Program pbrShader (vertexShaderCode->getRawCode(), fragmentShaderCode->getRawCode());
    pbrShader.use();
    pbrShader.setUniformBlockBinding("Camera", uboBindingPoint);
    pbrShader.setUniformBlockBinding("Material", matUboBindingPoint);

    Graphics::enable(Feature::DEPTH_TEST);

    // Set the render loop body
    window.setDrawCallback([&]()
    {
        Graphics::clearBuffers(BufferFlag::COLOR | BufferFlag::DEPTH);
        Graphics::drawElement(Primitive::TRIANGLES, engMesh.getNumDrawElements());
    });

    // Render
    window.renderLoop();

    return 0;
}
