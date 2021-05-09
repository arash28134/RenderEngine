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

template<>
std::unique_ptr<data::ShaderCode>
Resources::get<data::ShaderCode>(const String& path)
{

    const std::string stdPath = TOSTDSTR(path);
    std::ifstream file (stdPath);
    if(!file)
        throw std::runtime_error("ShaderCode reader: Could not open the file " + stdPath);

    try
    {
        return std::make_unique<data::ShaderCode>(
                    String( std::string(std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>()).c_str() ));
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("ShaderCode reader: Could not read the file " + stdPath + ": "
                                 + std::string(e.what()));
    }

    return {nullptr};
}

}
