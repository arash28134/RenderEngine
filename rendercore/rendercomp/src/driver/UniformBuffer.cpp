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
inline GLenum EngineToOpenGLBufferUsage(const BufferDataPolicy data,
                                        const BufferUsagePolicy usage) RC_NOEXCEPT
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

#ifdef RENDERCOMP_DEBUG
    throw std::runtime_error("Invalid Buffer data and/or usage policy");
#else
    return GL_INVALID_ENUM;
#endif
}

UniformBuffer::UniformBuffer(const size_t size,
                             const BufferDataPolicy data,
                             const BufferUsagePolicy usage) RC_NOEXCEPT
: _size(size)
, _handle(GL_INVALID_VALUE)
, _mapped(false)
{
#ifdef RENDERCOMP_DEBUG
   const size_t maxSize = _getMaxSize();
   if(_size > maxSize)
       throw std::runtime_error("Max allowed uniform buffer size is "
                                + std::to_string(maxSize));
#endif

   DRIVER_CALL(glCreateBuffers(1, &_handle));
   DRIVER_CALL(glNamedBufferData(_handle, size, nullptr, EngineToOpenGLBufferUsage(data, usage)));
}

UniformBuffer::~UniformBuffer()
{
    if(_handle != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteBuffers(1, &_handle));
}

void UniformBuffer::setData(const Vector<char> &byteData) const RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(byteData.size() > _size)
        throw std::runtime_error("Attempted to overflow uniform buffer "
                                 "with bigger data than buffer size");
#endif

    DRIVER_CALL(glNamedBufferSubData(_handle, 0, byteData.size(), (const void*)byteData.data()));
}

void UniformBuffer::writeData(const std::function<void(char* ptr, const
                                                       size_t bufferSize)>& cb) const RC_NOEXCEPT
{
    void* res = DRIVER_CALL(glMapNamedBuffer(_handle, GL_WRITE_ONLY));
    char* ptr = static_cast<char*>(res);
#ifdef RENDERCOMP_DEBUG
    try
    {
#endif
        cb(ptr, _size);
#ifdef RENDERCOMP_DEBUG
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::writeData(): " << e.what() << std::endl;
    }
#endif

    DRIVER_CALL(glUnmapNamedBuffer(_handle));
}

void UniformBuffer::readData(const std::function<void (const char *,
                                                       const size_t)> &cb) const RC_NOEXCEPT
{
    void* res = DRIVER_CALL(glMapNamedBuffer(_handle, GL_READ_ONLY));
    char* ptr = static_cast<char*>(res);
#ifdef RENDERCOMP_DEBUG
    try
    {
#endif
        cb(ptr, _size);
#ifdef RENDERCOMP_DEBUG
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::readData(): " << e.what() << std::endl;
    }
#endif
    DRIVER_CALL(glUnmapNamedBuffer(_handle));
}

void UniformBuffer::readWriteData(const std::function<void (char *,
                                                            const size_t)> &cb) const RC_NOEXCEPT
{
    void* res = DRIVER_CALL(glMapNamedBuffer(_handle, GL_READ_WRITE));
    char* ptr = static_cast<char*>(res);
#ifdef RENDERCOMP_DEBUG
    try
    {
#endif
        cb(ptr, _size);
#ifdef RENDERCOMP_DEBUG
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception on UniformBuffer::readWrite(): " << e.what() << std::endl;
    }
#endif
    DRIVER_CALL(glUnmapNamedBuffer(_handle));
}

void UniformBuffer::bindRangeToPoint(const size_t bindingPoint,
                                     const size_t offset,
                                     const size_t length) const RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(offset + length > _size)
        throw std::runtime_error("Tried to bind uniform buffer range beyond buffer size");
#endif

    DRIVER_CALL(glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, _handle, offset, length));
}

void UniformBuffer::bindToPoint(const size_t bindingPoint) const RC_NOEXCEPT
{
    DRIVER_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _handle));
}

size_t UniformBuffer::_getMaxSize() const RC_NOEXCEPT
{
    GLint maxSize;
    DRIVER_CALL(glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxSize));
    return static_cast<size_t>(maxSize);
}
}
