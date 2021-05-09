#include <rendercomp/common/Types.h>

#include <iostream>

int main(int, char**)
{
    using namespace rendercomp;

    const Vector<String> defines = {"MY_DEFINE", "ANOTHER=1"};
    String definesString;
    for(const auto& define : defines)
        definesString += "#define " + define + "\n";

    const String shaderCode = ""
        "//This is a comment\n"
        "\n"
        "#version 330 core\n"
        "uniform int myVar;\n"
        "void main() {\n"
        "   color=vec4(1);\n"
        "}";

    std::cout << shaderCode.c_str() << std::endl;

    auto versionPos = shaderCode.find("#version");
    auto newLineVersionPos = shaderCode.find('\n', versionPos);

    std::cout << "Version Pos = " << versionPos << ", newLinePos = " << newLineVersionPos << std::endl;

    String shaderHeader = shaderCode.substr(0, newLineVersionPos + 1);

    String shaderBody = shaderCode.substr(newLineVersionPos + 1);

    String modShader = shaderHeader + definesString + shaderBody;

    std::cout << std::endl << std::endl;

    std::cout << modShader.c_str() << std::endl;

    return 0;
}
