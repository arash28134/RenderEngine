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

#include <cstdint>
#include <cstdlib>

#include <glad/glad.h>

#include "../common/Types.h"

namespace rendercomp
{

enum class BufferFlag : uint32_t
{
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT
};

BufferFlag operator| (const BufferFlag left, const BufferFlag right);
BufferFlag& operator|= (BufferFlag& left, const BufferFlag right);
BufferFlag operator& (const BufferFlag left, const BufferFlag right);
BufferFlag& operator&= (BufferFlag& left, const BufferFlag right);
BufferFlag operator^ (const BufferFlag left, const BufferFlag right);
BufferFlag& operator^= (BufferFlag& left, const BufferFlag right);
BufferFlag operator~ (const BufferFlag flag);

enum class Primitive : uint32_t
{
    POINTS = GL_POINTS,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    LINES = GL_LINES,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_STIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    PATCHES = GL_PATCHES
};

struct IndirectCommand
{
    uint32_t count;
    uint32_t primCount;
    uint32_t firstIndex;
    uint32_t baseVertex;
    uint32_t baseInstance;
};

class Graphics
{
public:
    static void clearBuffers(const BufferFlag flagMask) noexcept;

    static void drawArray(const Primitive prim, const size_t offset, const size_t count) noexcept;

    static void drawArrayInstanced(const Primitive prim, const size_t offset,
                                   const size_t count, const size_t numElements) noexcept;

    static void drawElement(const Primitive prim, const size_t indexCount) noexcept;

    static void drawElementInstanced(const Primitive prim,
                                     const size_t indexCount,
                                     const size_t numElements) noexcept;

    static void drawElementIndirect(const Primitive prim, const IndirectCommand& command) noexcept;

    static void drawMultiElementsIndirect(const Primitive prim,
                                          const Vector<IndirectCommand>& commands) noexcept;

};
}
