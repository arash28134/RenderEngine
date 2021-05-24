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

#include <rendercomp/driver/Mesh.h>

namespace rendercomp
{
Mesh::Mesh() RC_NOEXCEPT
 : _vao(GL_INVALID_VALUE)
 , _ibo(GL_INVALID_VALUE)
 , _vbo(GL_INVALID_VALUE)
 , _numDrawVertex(0u)
{
    // Generate vao and bind it
    DRIVER_CALL(glCreateVertexArrays(1, &_vao));
}

Mesh::Mesh(data::Mesh* mesh, int32_t uvChannel, int32_t colorChannel) RC_NOEXCEPT
 : _vao(GL_INVALID_VALUE)
 , _ibo(GL_INVALID_VALUE)
 , _vbo(GL_INVALID_VALUE)
 , _numDrawVertex(0u)
{
#ifdef RENDERCOMP_DEBUG
    if(!mesh)
        throw std::runtime_error("Mesh: The given source Mesh object is null");

    if(uvChannel > -1 && static_cast<int32_t>(mesh->vertexUVs.size()) <= uvChannel)
        throw std::runtime_error("Mesh: Requeted uv channel "
                                 + std::to_string(uvChannel) + ", but only "
                                 + std::to_string(mesh->vertexUVs.size()) + "channels are "
                                 "available");

    if(colorChannel > -1 && static_cast<int32_t>(mesh->vertexColors.size()) <= uvChannel)
        throw std::runtime_error("Mesh: Requeted color channel "
                                 + std::to_string(uvChannel) + ", but only "
                                 + std::to_string(mesh->vertexUVs.size()) + "channels are "
                                 "available");
#endif

    // Generate vao and bind it
    DRIVER_CALL(glCreateVertexArrays(1, &_vao));

    // Generate face buffer
    DRIVER_CALL(glCreateBuffers(1, &_ibo));
    // Fill face buffer
    _setFaces(mesh);

    // Generate vertex attribute buffer
    DRIVER_CALL(glCreateBuffers(1, &_vbo));
    // Fill vertex attribute buffer
    _setVertexData(mesh, uvChannel, colorChannel);
}

Mesh::~Mesh()
{
    // Release buffers, if valid
    if(_ibo != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteBuffers(1, &_ibo));
    if(_vbo != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteBuffers(1, &_vbo));
    if(_vao != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteVertexArrays(1, &_vbo));
}

void Mesh::bind() const RC_NOEXCEPT
{
    DRIVER_CALL(glBindVertexArray(_vao));
}

void Mesh::_setFaces(data::Mesh* mesh) RC_NOEXCEPT
{
    if(mesh->faces.empty())
        return;

    // Fill the buffer with data from the Mesh object
    _numDrawVertex = mesh->faces.size() * 3;
    Vector<GLuint> faceBuffer (_numDrawVertex);
    for(size_t i = 0; i < mesh->faces.size(); ++i)
    {
        const size_t flatIdx = i * 3;
        faceBuffer[flatIdx]      = static_cast<GLuint>(mesh->faces[i].x);
        faceBuffer[flatIdx + 1]  = static_cast<GLuint>(mesh->faces[i].y);
        faceBuffer[flatIdx + 2]  = static_cast<GLuint>(mesh->faces[i].z);
    }

    // Allocate the space
    DRIVER_CALL(glNamedBufferStorage(_ibo,
                                     faceBuffer.size() * sizeof(GLuint),
                                     faceBuffer.data(),
                                     0));
    DRIVER_CALL(glVertexArrayElementBuffer(_vao, _ibo));

}

void Mesh::_setVertexData(data::Mesh* mesh, int32_t uvChannel, int32_t colorChannel) RC_NOEXCEPT
{
    // Compute total number of elements (floats) there will be in the buffer
    size_t bufferElements {0};
    bufferElements += 3;    // vertex positions (mandatory)
    bufferElements += mesh->vertexNormals.empty()? 0 : 3;
    bufferElements += mesh->vertexTangents.empty()? 0 : 3;

    // Check that the "requested" uv channel is there
    bool useUVs = false;
    if((uvChannel < 0 && mesh->vertexUVs.size() > 0) ||
            (uvChannel > -1 && mesh->vertexUVs.size() > static_cast<size_t>(uvChannel)))
    {
        bufferElements += 2;
        useUVs = true;
        uvChannel = std::max<int32_t>(0, uvChannel);
    }

    // Check that the "requested" color channel is there
    bool useColors = false;
    if((colorChannel < 0 && mesh->vertexColors.size() > 0) ||
            (colorChannel > -1 && mesh->vertexColors.size() > static_cast<size_t>(colorChannel)))
    {
        bufferElements += 4;
        useColors = true;
        colorChannel = std::max<int32_t>(0, colorChannel);
    }

    // Fill buffer with Mesh object information
    Vector<float> vertexBuffer (bufferElements * mesh->vertexPositions.size(), 0.f);
    for(size_t i = 0; i < mesh->vertexPositions.size(); ++i)
    {
        size_t vbIdx = i * bufferElements;
        vertexBuffer[vbIdx++] = mesh->vertexPositions[i].x;
        vertexBuffer[vbIdx++] = mesh->vertexPositions[i].y;
        vertexBuffer[vbIdx++] = mesh->vertexPositions[i].z;

        if(!mesh->vertexNormals.empty())
        {
            vertexBuffer[vbIdx++] = mesh->vertexNormals[i].x;
            vertexBuffer[vbIdx++] = mesh->vertexNormals[i].y;
            vertexBuffer[vbIdx++] = mesh->vertexNormals[i].z;
        }

        if(!mesh->vertexTangents.empty())
        {
            vertexBuffer[vbIdx++] = mesh->vertexTangents[i].x;
            vertexBuffer[vbIdx++] = mesh->vertexTangents[i].y;
            vertexBuffer[vbIdx++] = mesh->vertexTangents[i].z;
        }

        if(useUVs)
        {
            vertexBuffer[vbIdx++] = mesh->vertexUVs[uvChannel][i].x;
            vertexBuffer[vbIdx++] = mesh->vertexUVs[uvChannel][i].y;
        }
        if(useColors)
        {
            vertexBuffer[vbIdx++] = mesh->vertexColors[colorChannel][i].r;
            vertexBuffer[vbIdx++] = mesh->vertexColors[colorChannel][i].g;
            vertexBuffer[vbIdx++] = mesh->vertexColors[colorChannel][i].b;
            vertexBuffer[vbIdx++] = mesh->vertexColors[colorChannel][i].a;
        }
    }

    DRIVER_CALL(glNamedBufferStorage(_vbo,
                                     vertexBuffer.size() * sizeof(float),
                                     (const void*)vertexBuffer.data(),
                                     0));

    // Enable vertex attribute pointers for usage in shaders
    const size_t stride = bufferElements * sizeof(float);
    size_t offset = 0;

    // vertex positions
    DRIVER_CALL(glEnableVertexArrayAttrib(_vao, 0));
    DRIVER_CALL(glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, offset));
    DRIVER_CALL(glVertexArrayAttribBinding(_vao, 0, 0));
    offset += sizeof(float) * 3;

    // normal vectors
    if(!mesh->vertexNormals.empty())
    {
        DRIVER_CALL(glEnableVertexArrayAttrib(_vao, 1));
        DRIVER_CALL(glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, offset));
        DRIVER_CALL(glVertexArrayAttribBinding(_vao, 1, 0));
        offset += sizeof(float) * 3;
    }

    // Tangents (if any)
    if(!mesh->vertexTangents.empty())
    {
        DRIVER_CALL(glEnableVertexArrayAttrib(_vao, 2));
        DRIVER_CALL(glVertexArrayAttribFormat(_vao, 2, 3, GL_FLOAT, GL_FALSE, offset));
        DRIVER_CALL(glVertexArrayAttribBinding(_vao, 2, 0));
        offset += sizeof(float) * 3;
    }

    // UV map (if any)
    if(useUVs)
    {
        DRIVER_CALL(glEnableVertexArrayAttrib(_vao, 3));
        DRIVER_CALL(glVertexArrayAttribFormat(_vao, 3, 2, GL_FLOAT, GL_FALSE, offset));
        DRIVER_CALL(glVertexArrayAttribBinding(_vao, 3, 0));
        offset += sizeof(float) * 2;
    }

    // Vertex colors (if any)
    if(useColors)
    {
        DRIVER_CALL(glEnableVertexArrayAttrib(_vao, 4));
        DRIVER_CALL(glVertexArrayAttribFormat(_vao, 4, 4, GL_FLOAT, GL_FALSE, offset));
        DRIVER_CALL(glVertexArrayAttribBinding(_vao, 4, 0));
    }

    DRIVER_CALL(glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, stride));
}
}
