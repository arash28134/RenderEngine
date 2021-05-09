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

#include <rendercomp/driver/UniformBuffer.h>

#include <iostream>

namespace rendercomp
{
inline GLenum EngineToOpenGLBufferUsage(const BufferDataPolicy data, const BufferUsagePolicy usage)
{
    switch(data)
    {
    case BufferDataPolicy::STREAM:
        switch(usage)
        {
        case BufferUsagePolicy::DRAW:
            return GL_STREAM_DRAW;
            break;
        case BufferUsagePolicy::READ:
            return GL_STREAM_READ;
            break;
        case BufferUsagePolicy::COPY:
            return GL_STREAM_COPY;
            break;
        }
        break;
    case BufferDataPolicy::STATIC:
        switch(usage)
        {
        case BufferUsagePolicy::DRAW:
            return GL_STATIC_DRAW;
            break;
        case BufferUsagePolicy::READ:
            return GL_STATIC_READ;
            break;
        case BufferUsagePolicy::COPY:
            return GL_STATIC_COPY;
            break;
        }
        break;
    case BufferDataPolicy::DYNAMIC:
        switch(usage)
        {
        case BufferUsagePolicy::DRAW:
            return GL_DYNAMIC_DRAW;
            break;
        case BufferUsagePolicy::READ:
            return GL_DYNAMIC_READ;
            break;
        case BufferUsagePolicy::COPY:
            return GL_DYNAMIC_COPY;
            break;
        }
        break;
    }

    throw std::runtime_error("Invalid Buffer data and/or usage policy");
}

UniformBuffer::UniformBuffer(const size_t size,
                             const BufferDataPolicy data,
                             const BufferUsagePolicy usage)
: _size(size)
, _handle(GL_INVALID_VALUE)
, _mapped(false)
{
   const size_t maxSize = _getMaxSize();
   if(_size > maxSize)
       throw std::runtime_error("Max allowed uniform buffer size is "
                                + std::to_string(maxSize));

   glCreateBuffers(1, &_handle);
   glNamedBufferData(_handle, size, nullptr, EngineToOpenGLBufferUsage(data, usage));
}

UniformBuffer::~UniformBuffer()
{
    if(_handle != GL_INVALID_VALUE)
        glDeleteBuffers(1, &_handle);
}

void UniformBuffer::setData(const Vector<char> &byteData)
{
    if(byteData.size() > _size)
        throw std::runtime_error("Attempted to overflow uniform buffer "
                                 "with bigger data than buffer size");

    glNamedBufferSubData(_handle, 0, byteData.size(), (const void*)byteData.data());
}

void UniformBuffer::writeData(std::function<void(char* ptr, const size_t bufferSize)>& cb) noexcept
{
    char* ptr = static_cast<char*>(glMapNamedBuffer(_handle, GL_WRITE_ONLY));
    try
    {
        cb(ptr, _size);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::writeData(): " << e.what() << std::endl;
    }

    glUnmapNamedBuffer(_handle);
}

void UniformBuffer::readData(std::function<void (const char *, const size_t)> &cb) noexcept
{
    char* ptr = static_cast<char*>(glMapNamedBuffer(_handle, GL_READ_ONLY));
    try
    {
        cb(ptr, _size);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::readData(): " << e.what() << std::endl;
    }

    glUnmapNamedBuffer(_handle);
}

void UniformBuffer::readWriteData(std::function<void (char *, const size_t)> &cb) noexcept
{
    char* ptr = static_cast<char*>(glMapNamedBuffer(_handle, GL_READ_WRITE));
    try
    {
        cb(ptr, _size);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::readWrite(): " << e.what() << std::endl;
    }

    glUnmapNamedBuffer(_handle);
}

void UniformBuffer::bindRangeToPoint(const size_t bindingPoint,
                                     const size_t offset,
                                     const size_t length) const
{
    if(offset + length > _size)
        throw std::runtime_error("Tried to bind uniform buffer range beyond buffer size");

    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, _handle, offset, length);
}

void UniformBuffer::bindToPoint(const size_t bindingPoint) const noexcept
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _handle);
}

size_t UniformBuffer::_getMaxSize() const noexcept
{
    GLint maxSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxSize);
    return static_cast<size_t>(maxSize);
}
}
