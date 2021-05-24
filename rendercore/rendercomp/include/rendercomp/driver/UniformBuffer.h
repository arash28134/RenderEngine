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

#include "DriverDebug.h"

#include <glad/glad.h>

#include <functional>

namespace rendercomp
{
enum class BufferDataPolicy : uint8_t
{
    STREAM = 0,
    STATIC,
    DYNAMIC
};

enum class BufferUsagePolicy : uint8_t
{
    DRAW = 0,
    READ,
    COPY
};

/**
 * @brief Class that encapsulates the usage of a GPU Uniform buffer
 * @tparam usage A GLenum specifying the usage of this buffer. Might be GL_STREAM_DRAW,
 *         GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
 *         GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.
 */
class UniformBuffer
{
public:
    /**
     * @brief UniformBuffer Creates and allocates a GPU Uniform buffer to be used in a shader.
     * @param byteSize The size in bytes of this buffer
     * @param data The policy of how the data will be updated on this buffer
     * @param usage The policy of how this buffer will be used
     * @throws std::runtime_error if the requested size exceeds the max size of an uniform buffer.
     */
    UniformBuffer(const size_t byteSize,
                  const BufferDataPolicy data,
                  const BufferUsagePolicy usage) RC_NOEXCEPT;

    ~UniformBuffer();

    /**
     * @brief setData Upload the givenb uffer data to this GPU buffer
     * @param byteData A byte buffer containing raw data.
     * @throws std::runtime_error if the given buffer exceeds this buffer size
     */
    void setData(const Vector<char>& byteData) const RC_NOEXCEPT;

    /**
     * @brief Allows to specify a callback, where a pointer to the gpu memory of this
     *        buffer, and its size, are provided to allow to write data to it.
     * @param cb Callback with the form void(char* ptr, const size_t size) that will
     *        write data to the provided ptr.
     */
    void writeData(const std::function<void(char* ptr,
                                            const size_t bufferSize)>& cb) const RC_NOEXCEPT;

    /**
     * @brief Allows to specify a callback, where a pointer to the gpu memory of this
     *        buffer, and its size, are provided to allow to read data from it.
     * @param cb Callback with the form void(char* ptr, const size_t size) that will
     *        read data from the provided ptr.
     */
    void readData(const std::function<void(const char* ptr,
                                           const size_t bufferSize)>& cb) const RC_NOEXCEPT;

    /**
     * @brief Allows to specify a callback, where a pointer to the gpu memory of this
     *        buffer, and its size, are provided to allow to write and read data to/from it.
     * @param cb Callback with the form void(char* ptr, const size_t size) that will
     *        write and/or read data to the provided ptr.
     */
    void readWriteData(const std::function<void(char* ptr,
                                                const size_t bufferSize)>& cb) const RC_NOEXCEPT;

    /**
     * @brief bindToPoint Binds the buffer to the given binding point to be used by a shader
     * @param bindingPoint The binding point at which to bind this buffer.
     */
    void bindToPoint(const size_t bindingPoint) const RC_NOEXCEPT;

    /**
     * @brief bindRangeToPoint Binds a part of this buffer's data (specified by the offset and
     *        length) to the given binding point, to bue used by a shader.
     * @param bindingPoint The binding point to which to bind the buffer data
     * @param offset Byte offset from the beggining of buffer's data at which to start the
     *        binding
     * @param length Byte lenght from the offset specifying how many bytes to bind
     * @throws std::runtime_error if the offset + length is beyond this buffers size
     */
    void bindRangeToPoint(const size_t bindingPoint,
                          const size_t offset,
                          const size_t length) const RC_NOEXCEPT;

    size_t getSize() const noexcept
    {
        return _size;
    }

private:
    /**
     * @brief getMaxSize Queries the OpenGL implementation to know what is the maximum allowed
     *        size in bytes for an uniform buffer
     * @return size_t The maximum uniform buffer size in bytes.
     */
    size_t _getMaxSize() const RC_NOEXCEPT;

private:
    const size_t _size;
    GLuint _handle;
    bool _mapped;
};
}
