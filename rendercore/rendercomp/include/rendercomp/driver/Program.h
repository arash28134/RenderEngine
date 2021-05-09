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

#pragma once

#include "../common/Types.h"

#include <glad/glad.h>

namespace rendercomp
{

class Program
{
public:
    /**
     * @brief Program constructor. Creates a new OpenGL program with the given vertex and fragment
     *        shader codes to enable the vertex and fragment stages of the pipeline.
     * @param vertexShader GLSL code for the vertex shader.
     * @param fragmentShader GLSL code for the fragment shader.
     * @throws std::runtime_error if any shader code is empty, or if the compilation or linkage
     *         processes fail.
     */
    Program(const String& vertexShader, const String& fragmentShader);

    /**
     * @brief Program constructor. Creates a new OpenGL program with the given vertex, geometry
     *        and fragment shader codes to enable the vertex, geometry and fragment stages
     *        of the pipeline.
     * @param vertexShader GLSL code for the vertex shader.
     * @param geometryShader GLSL code for the geometry shader.
     * @param fragmentShader GLSL code for the fragment shader.
     * @throws std::runtime_error if any shader code is empty, or if the compilation or linkage
     *         processes fail.
     */
    Program(const String& vertexShader, const String& geometryShader,
            const String& fragmentShader);

    /**
     * @brief Program constructor. Creates a new OpenGL program with the given vertex, tesselation
     *        control, tesselation evaluation and fragment shader codes to enable the vertex,
     *        tesselation and fragment stages of the pipeline.
     * @param vertexShader GLSL code for the vertex shader.
     * @param tessCtrlShader GLSL code for the tesselation control shader.
     * @param tessEvalShader GLSL code for the tesselation evaluation shader.
     * @param fragmentShader GLSL code for the fragment shader.
     * @throws std::runtime_error if any shader code is empty, or if the compilation or linkage
     *         processes fail.
     */
    Program(const String& vertexShader, const String& tessCtrlShader,
            const String& tessEvalShader, const String& fragmentShader);

    /**
     * @brief Program constructor. Creates a new OpenGL program with the given vertex, tesselation
     *        control, tesselation evaluation, geometry and fragment shader codes to enable the
     *        vertex, tesselation, geometry and fragment stages of the pipeline.
     * @param vertexShader GLSL code for the vertex shader.
     * @param tessCtrlShader GLSL code for the tesselation control shader.
     * @param tessEvalShader GLSL code for the tesselation evaluation shader.
     * @param geometryShader GLSL code for the geometry shader.
     * @param fragmentShader GLSL code for the fragment shader.
     * @throws std::runtime_error if any shader code is empty, or if the compilation or linkage
     *         processes fail.
     */
    Program(const String& vertexShader, const String& tessCtrlShader,
            const String& tessEvalShader, const String& geomShader,
            const String& fragmentShader);

    ~Program();

    /**
     * @brief use Indicates the current OpenGL context to use this program for any subsequent
     *        drawing call.
     */
    void use() const;

    /**
     * @brief getUniform Retrieves the location of the uniform named 'name' on this program
     * @param name String representing the name of the uniform in the shader code
     * @return the location of the uniform
     */
    uint32_t getUniform(const String& name);

    /**
     * @brief setUniformBlockBinding Sets the binding point of the uniform block
     *        represented by its name on the shader code
     * @param name String representing the name of the uniform block in the shader code
     * @param bindingPoint the integer index in which the uniform block should be binded
     */
    void setUniformBlockBinding(const String& name, const uint32_t bindingPoint);

    template<typename T>
    void setUniform(const uint32_t, const T&)
    {
        throw std::runtime_error("No implementation to set uniforms of type "
                                  + std::string(typeid(T).name()));
    }

private:
    /**
     * @brief _initialize Initializes the program by compiling and attaching the shaders which
     *        are indicated as an associative map of shader type and shader code
     * @param shaderCodes Map containing each of the shader types that must be attached to this
     *        program, and the code of each of them.
     * @throws std::runtime_error if any error arises from compiling individual shader code or
     *         during program linkage
     */
    void _initialize(const Map<GLenum, const String&> shaderCodes);

    /**
     * @brief _compileShader Compiles the given type of shader using the provided
     *        String with the shader code
     * @param type GLenum indicating the type of shader being compiled
     * @param code String containing the shader sourcecode
     * @return The OpenGL shader handle
     * @throws std::runtime_error if there is any error during shader compilation
     */
    GLuint _compileShader(const GLenum type, const String& code);

    /**
     * @brief _checkLinkage Checkes wether the linkage process of the program was
     *        successful, throws an exception otherwise
     * @throws std::runtime_error if there was any error linking the program.
     */
    void _checkLinkage();

private:
    GLuint _programHandle;

};

template<>
void Program::setUniform<float>(const uint32_t uniform, const float& value);
template<>
void Program::setUniform<Vec2f>(const uint32_t uniform, const Vec2f& value);
template<>
void Program::setUniform<Vec3f>(const uint32_t uniform, const Vec3f& value);
template<>
void Program::setUniform<Vec4f>(const uint32_t uniform, const Vec4f& value);
template<>
void Program::setUniform<int32_t>(const uint32_t uniform, const int32_t& value);
template<>
void Program::setUniform<Vec2i>(const uint32_t uniform, const Vec2i& value);
template<>
void Program::setUniform<Vec3i>(const uint32_t uniform, const Vec3i& value);
template<>
void Program::setUniform<Vec4i>(const uint32_t uniform, const Vec4i& value);
template<>
void Program::setUniform<uint32_t>(const uint32_t uniform, const uint32_t& value);
template<>
void Program::setUniform<Vec2ui>(const uint32_t uniform, const Vec2ui& value);
template<>
void Program::setUniform<Vec3ui>(const uint32_t uniform, const Vec3ui& value);
template<>
void Program::setUniform<Vec4ui>(const uint32_t uniform, const Vec4ui& value);
template<>
void Program::setUniform<Mat3>(const uint32_t uniform, const Mat3& value);
template<>
void Program::setUniform<Mat4>(const uint32_t uniform, const Mat4& value);
}
