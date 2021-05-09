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

#include <rendercomp/driver/Program.h>

namespace rendercomp
{

void DeleteShaders(const Vector<GLuint>& handles)
{
    for(const auto shaderHandle : handles)
    {
        if(shaderHandle != GL_INVALID_VALUE)
            glDeleteShader(shaderHandle);
    }
}

Program::Program(const String& vertexShader, const String& fragmentShader)
 : _programHandle(GL_INVALID_VALUE)
{
    if(vertexShader.empty())
        throw std::runtime_error("Vertex shader must be a valid GLSL code");
    if(fragmentShader.empty())
        throw std::runtime_error("Fragment shader must be a valid GLSL code");

    _initialize({{GL_VERTEX_SHADER, vertexShader},
                 {GL_FRAGMENT_SHADER, fragmentShader}});
}

Program::Program(const String& vertexShader, const String& geometryShader,
                 const String& fragmentShader)
 : _programHandle(GL_INVALID_VALUE)
{
    if(vertexShader.empty())
        throw std::runtime_error("Vertex shader must be a valid GLSL code");
    if(geometryShader.empty())
        throw std::runtime_error("Geometry shader must be a valid GLSL code");
    if(fragmentShader.empty())
        throw std::runtime_error("Fragment shader must be a valid GLSL code");

    _initialize({{GL_VERTEX_SHADER, vertexShader},
                 {GL_GEOMETRY_SHADER, geometryShader},
                 {GL_FRAGMENT_SHADER, fragmentShader}});
}

Program::Program(const String& vertexShader, const String& tessCtrlShader,
                 const String& tessEvalShader, const String& fragmentShader)
 : _programHandle(GL_INVALID_VALUE)
{
    if(vertexShader.empty())
        throw std::runtime_error("Vertex shader must be a valid GLSL code");
    if(tessCtrlShader.empty())
        throw std::runtime_error("Tesselation control shader must be a valid GLSL code");
    if(tessEvalShader.empty())
        throw std::runtime_error("Tesselation evaluation shader must be a valid GLSL code");
    if(fragmentShader.empty())
        throw std::runtime_error("Fragment shader must be a valid GLSL code");

    _initialize({{GL_VERTEX_SHADER, vertexShader},
                 {GL_TESS_CONTROL_SHADER, tessCtrlShader},
                 {GL_TESS_EVALUATION_SHADER, tessEvalShader},
                 {GL_FRAGMENT_SHADER, fragmentShader}});
}

Program::Program(const String& vertexShader, const String& tessCtrlShader,
                 const String& tessEvalShader, const String& geomShader,
                 const String& fragmentShader)
 : _programHandle(GL_INVALID_VALUE)
{
    if(vertexShader.empty())
        throw std::runtime_error("Vertex shader must be a valid GLSL code");
    if(tessCtrlShader.empty())
        throw std::runtime_error("Tesselation control shader must be a valid GLSL code");
    if(tessEvalShader.empty())
        throw std::runtime_error("Tesselation evaluation shader must be a valid GLSL code");
    if(geomShader.empty())
        throw std::runtime_error("Geometry shader must be a valid GLSL code");
    if(fragmentShader.empty())
        throw std::runtime_error("Fragment shader must be a valid GLSL code");

    _initialize({{GL_VERTEX_SHADER, vertexShader},
                 {GL_TESS_CONTROL_SHADER, tessCtrlShader},
                 {GL_TESS_EVALUATION_SHADER, tessEvalShader},
                 {GL_GEOMETRY_SHADER, geomShader},
                 {GL_FRAGMENT_SHADER, fragmentShader}});
}

Program::~Program()
{
    if(_programHandle != GL_INVALID_VALUE)
        glDeleteProgram(_programHandle);
}

void Program::use() const
{
    glUseProgram(_programHandle);
}

void Program::_initialize(const Map<GLenum, const String&> shaderCodes)
{
    // Create the program
    _programHandle = glCreateProgram();

    // Try to compile the shaders and attach them to the program
    auto it = shaderCodes.begin();
    Vector<GLuint> shaderHandles;
    shaderHandles.reserve(shaderCodes.size());
    for(; it != shaderCodes.end(); ++it)
    {
        try
        {
            const GLuint shaderHandle = _compileShader(it->first, it->second);
            shaderHandles.push_back(shaderHandle);
            glAttachShader(_programHandle, shaderHandle);
        }
        catch (const std::exception& e)
        {
            // In the event of an error, clean up any successfully compiled shader prior to this
            // one
            DeleteShaders(shaderHandles);
            throw std::runtime_error("Error while compiling shader:\n" + std::string(e.what()));
        }
    }

    // Try to link the program
    glLinkProgram(_programHandle);
    try
    {
        _checkLinkage();
    }
    catch(std::exception& e)
    {
        // In the event of a linkage failure, clear the compiled shaders
        DeleteShaders(shaderHandles);
        throw std::runtime_error("Error while linking program:\n" + std::string(e.what()));
    }

    // After everything is successful, clean the shader codes
    DeleteShaders(shaderHandles);
}

GLuint Program::_compileShader(const GLenum type, const String& code)
{
    GLuint shader = glCreateShader(type);
    const size_t codeLen = code.length();
    const char* rawCode = code.data();
    glShaderSource(shader, 1, (const GLchar **)&rawCode, (const GLint *)&codeLen);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::string logString (logLen, '\0');
        glGetShaderInfoLog(shader, logLen, NULL, (GLchar*)logString.data());
        glDeleteShader(shader);
        throw std::runtime_error(logString);
    }

    return shader;
}

void Program::_checkLinkage()
{
    int32_t linked;
    glGetProgramiv(_programHandle, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint logLen;
        glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &logLen);
        std::string logString (static_cast<std::string::size_type>(logLen), '\0');
        glGetProgramInfoLog(_programHandle, logLen, NULL, (GLchar*)logString.data());
        throw std::runtime_error(logString);
    }
}

template<>
void Program::setUniform<float>(const uint32_t uniform, const float& value)
{
    glUniform1f(uniform,  value);
}

template<>
void Program::setUniform<Vec2f>(const uint32_t uniform, const Vec2f& value)
{
    glUniform2fv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec3f>(const uint32_t uniform, const Vec3f& value)
{
    glUniform3fv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec4f>(const uint32_t uniform, const Vec4f& value)
{
    glUniform4fv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<int32_t>(const uint32_t uniform, const int32_t& value)
{
    glUniform1i(uniform, value);
}

template<>
void Program::setUniform<Vec2i>(const uint32_t uniform, const Vec2i& value)
{
    glUniform2iv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec3i>(const uint32_t uniform, const Vec3i& value)
{
    glUniform3iv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec4i>(const uint32_t uniform, const Vec4i& value)
{
    glUniform4iv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<uint32_t>(const uint32_t uniform, const uint32_t& value)
{
    glUniform1ui(uniform, value);
}

template<>
void Program::setUniform<Vec2ui>(const uint32_t uniform, const Vec2ui& value)
{
    glUniform2uiv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec3ui>(const uint32_t uniform, const Vec3ui& value)
{
    glUniform3uiv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Vec4ui>(const uint32_t uniform, const Vec4ui& value)
{
    glUniform4uiv(uniform, 1, &value[0]);
}

template<>
void Program::setUniform<Mat3>(const uint32_t uniform, const Mat3& value)
{
    glUniformMatrix3fv(uniform, 1, GL_FALSE, &value[0][0]);
}

template<>
void Program::setUniform<Mat4>(const uint32_t uniform, const Mat4& value)
{
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &value[0][0]);
}

void Program::setUniformBlockBinding(const String& name, const uint32_t bindingPoint)
{
    const GLuint blockIndex = glGetUniformBlockIndex(_programHandle, name.c_str());
    glUniformBlockBinding(_programHandle, blockIndex, bindingPoint);
}
}
