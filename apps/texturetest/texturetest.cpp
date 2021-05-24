#include <rendercomp/core/resources/Image.h>
#include <rendercomp/core/resources/Mesh.h>
#include <rendercomp/core/resources/ShaderCode.h>

#include <rendercomp/driver/Graphics.h>
#include <rendercomp/driver/Mesh.h>
#include <rendercomp/driver/Program.h>
#include <rendercomp/driver/Texture2D.h>
#include <rendercomp/driver/Window.h>

#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path to image>" << std::endl;
        return 0;
    }

    using namespace rendercomp;

    FilesystemUtils::init(argv[0]);

    // Load image
    const String path (argv[1]);
    const auto image = Resources::load<data::Image>(path);

    // Create a window which respects image aspect ratio
    const float aspectRatio = static_cast<float>(image->width) / static_cast<float>(image->height);
    WindowConfiguration config;
    config.title = "Image viewer";
    config.width = 1536;
    config.height = static_cast<uint32_t>(1536.0f / aspectRatio);
    Window window (config);

    // Create a dummy mesh
    const Mesh mesh;
    mesh.bind();

    // Create a program to display the image
    const auto vertShader = Resources::load<data::ShaderCode>(
                String(FilesystemUtils::currentPath() + "/texture.vert"));
    const auto fragShader = Resources::load<data::ShaderCode>(
                String(FilesystemUtils::currentPath() + "/texture.frag"));
    const Program program (vertShader->getRawCode(), fragShader->getRawCode());
    program.use();

    // Create the texture
    Texture2D texture (image.get(), 1);
    texture.setMinFilter(TextureMinFilter::LINEAR);
    texture.bind(0);

    // Disable depth test
    Graphics::disable(Feature::DEPTH_TEST);

    // Draw callback
    window.setDrawCallback([&]()
    {
        Graphics::clearBuffers(BufferFlag::COLOR);
        Graphics::drawArray(Primitive::TRIANGLE_STRIP, 0, 4);
    });

    // Do render
    window.renderLoop();

    return 0;
}
