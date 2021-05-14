/*
 * This file is part of the RenderEngine distribution (https://github.com/NadirRoGue/RenderEngine).
 * Copyright (c) 2021 Nadir Román Guerrero.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <rendercomp/core/resources/ShaderCode.h>

#include <fstream>

#define MAX_SHADER_INCLUDE_DEPTH 64

namespace rendercomp
{
namespace data
{
ShaderCode::ShaderCode(const String& code)
 : _code(code)
{
}

const String& ShaderCode::getRawCode() const
{
    return _code;
}

String ShaderCode::parseUBERCode(const Vector<String> &defines)
{
    const auto versionPos = _code.find("#version");
    if(versionPos == _code.npos)
        throw std::runtime_error("Shader code do not include the #version header");

    const auto versionNewLinePos = _code.find('\n', versionPos);

    const String shaderHeader = _code.substr(0, versionNewLinePos + 1);
    const String shaderBody = _code.substr(versionNewLinePos + 1);

    String defineList;
    for(const String& def : defines)
        defineList += "#define " + def + "\n";

    return shaderHeader + defineList + shaderBody;
}

}

std::string LoadShaderCode(const std::string& path, uint32_t depth = 0)
{
    if(depth > MAX_SHADER_INCLUDE_DEPTH)
        throw std::runtime_error("ShaderCode reader: Max include depth ("
                                 + std::to_string(MAX_SHADER_INCLUDE_DEPTH) + " reached");
    std::ifstream file (path);
    if(!file)
        throw std::runtime_error("ShaderCode reader: Could not open the file " + path);

    const std::string basePath =
            std::string(FilesystemUtils::getParentDirectory(String(path.c_str())).c_str());

    std::string rawCode;
    try
    {
        std::string buf;
        size_t line = 0;
        while(std::getline(file, buf))
        {
            if(buf.find("#include") == 0)
            {
                const std::string::size_type spacePos = buf.find(' ');
                if(spacePos == std::string::npos)
                    throw std::runtime_error("Cannot find space separator on #include clause "
                                             "on line " + std::to_string(line));

                std::string shaderIncludePath = basePath + buf.substr(spacePos + 1);
                if(!FilesystemUtils::exists(String(shaderIncludePath.c_str())))
                    throw std::runtime_error("ShaderCode reader: Cannot find shader from #include "
                                             "clause \n\t" + buf + "\non path \n\t"
                                             + shaderIncludePath);

                buf = LoadShaderCode(shaderIncludePath, depth + 1);
            }

            rawCode += buf + "\n";
        }
        //code =  String( std::string(std::istreambuf_iterator<char>(file),
        //                std::istreambuf_iterator<char>()).c_str() );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("ShaderCode reader: Could not read the file " + path + ": "
                                 + std::string(e.what()));
    }

    return rawCode;
}

template<>
std::unique_ptr<data::ShaderCode>
Resources::get<data::ShaderCode>(const String& path)
{
    return std::make_unique<data::ShaderCode>(
                String(LoadShaderCode(std::string(path.c_str())).c_str()));
}

}
