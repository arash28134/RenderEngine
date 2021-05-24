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

#include "../core/resources/Mesh.h"
#include "DriverDebug.h"

#include <glad/glad.h>

namespace rendercomp
{
class Mesh
{
public:

    /**
     * @brief Creates an empty mesh.
     */
    Mesh() RC_NOEXCEPT;

    /**
     * @brief Allocates the needed GPU buffers and upload the mesh data to them.
     *        This is a RAII type class, for which whatever resources are
     *        necessary, they must be allocated in the constructor, and released in the destructor.
     * @param mesh Mesh object which contains source data for the engine specific implementation
     * @param uvChannel index of the uv channel to use. A value of -1 indicates to either ignore
     *        if the mesh has no uv channels, or to pick index 0 if available. Any other value
     *        greater or equal to 0 will attempt to fetch that uv channel, throwing if its not
     *        present. Any value less than -1 has no meaning and will be campled to -1
     * @param colorChannel index of the color channel to use. A value of -1 indicates to either
     *        ignore if the mesh has no color channels, or to pick index 0 if available.
     *        Any other value greater or equal to 0 will attempt to fetch that uv channel,
     *        throwing if its not present. Any value less than -1 has no meaning and will be
     *        campled to -1
     * @throws std::runtime_error if the mesh is null, or if the uvChannel/colorChannel do not
     *         match what is available in the mesh
     */
    Mesh(data::Mesh* mesh, int32_t uvChannel = -1, int32_t colorChannel = -1) RC_NOEXCEPT;

    ~Mesh();

    void bind() const RC_NOEXCEPT;

    size_t getNumDrawElements() const RC_NOEXCEPT
    {
        return _numDrawVertex;
    }

private:
    /**
     * @brief _setFaces Allocates the needed space in the element buffer and sets the data
     *        using the Mesh object as source
     * @param mesh Mesh object containing the face information
     */
    void _setFaces(data::Mesh* mesh) RC_NOEXCEPT;

    /**
     * @brief _setVertexData Allocates the needed space in the array buffer and sets the
     *        data using the Mesh object as source, in an interlaced way (in other words,
     *        the buffer has the following layout: vvvnnn{ttt}{uu}{cccc}vvvnnn{ttt}...,
     *        with v = vertex pos, n = vertex normal, t = vertex tangent if any,
     *        u = uv coordinates if any, c = vertex colors if any).
     * @param mesh Mesh object containing the vertex information
     * @param uvChannel index of the uv channel to use.
     * @param colorChannel index of the color channel to use.
     */
    void _setVertexData(data::Mesh* mesh, int32_t uvChannel, int32_t colorChannel) RC_NOEXCEPT;

    GLuint _vao;
    GLuint _ibo;
    GLuint _vbo;
    size_t _numDrawVertex;
};
}
