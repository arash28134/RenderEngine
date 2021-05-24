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

#include "DriverDebug.h"

namespace rendercomp
{

/**
 * @brief The BufferFlag enum
 * The BufferFlag is a bitwise operation enabled enum which is used
 * to specify to the Graphics class which buffers from the device to
 * clean when issuing Graphics::clearBuffers() call
 */
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

/**
 * @brief The Primitive enum
 * The Primitive enum contains the available type of primitives to be rendere by the
 * underlying graphics driver. They are used on the calls to Graphics::draw* functions.
 */
enum class Primitive : uint32_t
{
    POINTS = GL_POINTS,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    LINES = GL_LINES,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    PATCHES = GL_PATCHES
};

enum class Feature : uint32_t
{
    // source and destination fragment blending
    BLEND = GL_BLEND,
    // Apply currently selected logic operation to fragment and buffer colors
    COLOR_LOGIC_OP = GL_COLOR_LOGIC_OP,
    // Cull faces facing away the projection plane
    CULL_FACE = GL_CULL_FACE,
    // Depth clamped by near and far plane
    DEPTH_CLAMP = GL_DEPTH_CLAMP,
    // Fragment discard based on depth value
    DEPTH_TEST = GL_DEPTH_TEST,
    // Dither (adds noise) to color values or indices before written to the color buffer
    DITHER = GL_DITHER,
    // Filter line rendering for smooth results
    LINE_SMOOTH = GL_LINE_SMOOTH,
    // Use multiple fragments to compute a single final color
    MULTISAMPLE = GL_MULTISAMPLE,
    // Adds an offset to the depth of filled rendered polygons
    POLYGON_OFFSET_FILL = GL_POLYGON_OFFSET_FILL,
    // Adds an offset to the depth of line rendered polygons
    POLYGON_OFFSET_LINE = GL_POLYGON_OFFSET_LINE,
    // Adds an offset to the depth of point rendered polygons
    POLYGON_OFFSET_POINT = GL_POLYGON_OFFSET_POINT,
    // Filter polygon rendering for smooth results
    POLYGON_SMOOTH = GL_POLYGON_SMOOTH,
    // Discard fragments outside the scissor rectangle
    SCISSOR_TEST = GL_SCISSOR_TEST,
    // Stencil test
    STENCIL_TEST = GL_STENCIL_TEST,
    // Eanble shader point size values
    PROGRAM_POINT_SIZE = GL_PROGRAM_POINT_SIZE
};

/**
 * @brief The IndirectCommand struct
 * The IndirectCommand struct is a small buffer with the configuraiton
 * to issue indirect draw calls
 */
struct IndirectCommand
{
    uint32_t count;
    uint32_t primCount;
    uint32_t firstIndex;
    uint32_t baseVertex;
    uint32_t baseInstance;
};

/**
 * @brief The Graphics class
 * The Graphics class encapsulates the calls to configure, clear and draw commands
 * of the underlying graphics driver. All the draw* methods expect an appropiate buffer
 * to be binded for the command to be sucessful, as no client side geometry data might
 * be passed on to them.
 */
class Graphics
{
public:
    /**
     * @brief Enables a built-in feature of the rendering pipline
     * @param A valid feature enum key
     */
    static void enable(const Feature feature) RC_NOEXCEPT;

    /**
     * @brief Disables a built-in feature of the rendering pipline
     * @param A valid feature enum key
     */
    static void disable(const Feature feature) RC_NOEXCEPT;

    /**
     * @brief clearBuffers Issues a clear buffer command for the underlying graphics driver.
     * @param flagMask A bitwise combination of the BufferFlag enum keys.
     */
    static void clearBuffers(const BufferFlag flagMask) RC_NOEXCEPT;

    /**
     * @brief drawArray Draws a list of vertices from a previsouly binded buffer.
     * @param prim Type of primitive to render with the given vertices
     * @param offset Offset within the binded buffer at which to start fetching vertices for
     *        rendering
     * @param count number of vertices to render (Careful not to overflow the read, as the
     *        count starts at the offset parameter)
     */
    static void drawArray(const Primitive prim,
                          const size_t offset,
                          const size_t count) RC_NOEXCEPT;

    /**
     * @brief drawArrayInstanced Draws a specified number of instances of the vertices which are
     *        provided by a previosuly binded buffer
     * @param prim Type of primitive to render with the given vertices
     * @param offset Offset within the binded buffer at which to start fetching vertices for
     *        rendering
     * @param count number of vertices to render (Careful not to overflow the read, as the
     *        count starts at the offset parameter)
     * @param numElements number of instances to render of the given vertices.
     */
    static void drawArrayInstanced(const Primitive prim,
                                   const size_t offset,
                                   const size_t count,
                                   const size_t numElements) RC_NOEXCEPT;

    /**
     * @brief drawElement Draws an indexed set of vertices from a previously binded mesh
     * @param prim Type of primitive to render with the vertex data
     * @param indexCount Number of indices that will be renderer.
     */
    static void drawElement(const Primitive prim, const size_t indexCount) RC_NOEXCEPT;

    /**
     * @brief drawElement Draws a specified number of instances of an indexed set
     *        of vertices from a previously binded mesh
     * @param prim Type of primitive to render with the vertex data
     * @param indexCount Number of indices that will be renderer.
     * @param numElements number of instances to ender of the given vertices
     */
    static void drawElementInstanced(const Primitive prim,
                                     const size_t indexCount,
                                     const size_t numElements) RC_NOEXCEPT;

    /**
     * @brief drawElementIndirect Draws a specified number of instances of an indexed
     *        set of vertices from a previously binded mesh, using an indirect buffer to
     *        configure the draw call
     * @param prim Type of primitive to render with the given vertices
     * @param command The configuration for the draw call
     */
    static void drawElementIndirect(const Primitive prim,
                                    const IndirectCommand& command) RC_NOEXCEPT;

    /**
     * @brief drawElementIndirect Draws a specified number of instances of an indexed
     *        set of vertices from a previously binded mesh, using multiple indirect buffers to
     *        configure the draw call
     * @param prim Type of primitive to render with the given vertices
     * @param commands A list of configurations for the draw call
     */
    static void drawMultiElementsIndirect(const Primitive prim,
                                          const Vector<IndirectCommand>& commands) RC_NOEXCEPT;

};
}
