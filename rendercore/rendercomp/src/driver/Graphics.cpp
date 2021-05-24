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

#include <rendercomp/driver/Graphics.h>

namespace rendercomp
{
BufferFlag operator| (const BufferFlag left, const BufferFlag right)
{
    return static_cast<BufferFlag>(
                static_cast<uint32_t>(left) | static_cast<uint32_t>(right)
                );
}

BufferFlag& operator|= (BufferFlag& left, const BufferFlag right)
{
    left = static_cast<BufferFlag>(static_cast<uint32_t>(left) | static_cast<uint32_t>(right));
    return left;
}

BufferFlag operator& (const BufferFlag left, const BufferFlag right)
{
    return static_cast<BufferFlag>(
                static_cast<uint32_t>(left) & static_cast<uint32_t>(right)
                );
}

BufferFlag& operator&= (BufferFlag& left, const BufferFlag right)
{
    left = static_cast<BufferFlag>(static_cast<uint32_t>(left) & static_cast<uint32_t>(right));
    return left;
}

BufferFlag operator^ (const BufferFlag left, const BufferFlag right)
{
    return static_cast<BufferFlag>(
                static_cast<uint32_t>(left) ^ static_cast<uint32_t>(right)
                );
}

BufferFlag& operator^= (BufferFlag& left, const BufferFlag right)
{
    left = static_cast<BufferFlag>(static_cast<uint32_t>(left) ^ static_cast<uint32_t>(right));
    return left;
}

BufferFlag operator~ (const BufferFlag flag)
{
    return static_cast<BufferFlag>(~static_cast<uint32_t>(flag));
}

void Graphics::enable(const Feature feature) RC_NOEXCEPT
{
    DRIVER_CALL(glEnable(static_cast<uint32_t>(feature)));
}

void Graphics::disable(const Feature feature) RC_NOEXCEPT
{
    DRIVER_CALL(glDisable(static_cast<uint32_t>(feature)));
}

void Graphics::clearBuffers(const BufferFlag flagMask) RC_NOEXCEPT
{
    DRIVER_CALL(glClear(static_cast<uint32_t>(flagMask)));
}

void Graphics::drawArray(const Primitive prim, const size_t offset, const size_t count) RC_NOEXCEPT
{
    DRIVER_CALL(glDrawArrays(static_cast<uint32_t>(prim), offset, count));
}

void Graphics::drawArrayInstanced(const Primitive prim, const size_t offset,
                                  const size_t count, const size_t numElements) RC_NOEXCEPT
{
    DRIVER_CALL(glDrawArraysInstanced(static_cast<uint32_t>(prim), offset, count, numElements));
}

void Graphics::drawElement(const Primitive prim, const size_t indexCount) RC_NOEXCEPT
{
    DRIVER_CALL(glDrawElements(static_cast<uint32_t>(prim),
                               indexCount,
                               GL_UNSIGNED_INT,
                               (const void*)0));
}

void Graphics::drawElementInstanced(const Primitive prim, const size_t indexCount,
                                    const size_t numElements) RC_NOEXCEPT
{
    DRIVER_CALL(glDrawElementsInstanced(static_cast<uint32_t>(prim),
                                        indexCount,
                                        GL_UNSIGNED_INT,
                                        (const void*)0,
                                        numElements));
}

void Graphics::drawElementIndirect(const Primitive prim,
                                   const IndirectCommand &command) RC_NOEXCEPT
{
    DRIVER_CALL(glDrawElementsIndirect(static_cast<uint32_t>(prim), GL_UNSIGNED_INT, &command));
}

void Graphics::drawMultiElementsIndirect(const Primitive prim,
                                         const Vector<IndirectCommand> &commands) RC_NOEXCEPT
{
    DRIVER_CALL(glMultiDrawElementsIndirect(static_cast<uint32_t>(prim),
                                            GL_UNSIGNED_INT,
                                            commands.data(),
                                            commands.size(),
                                            0));
}
}
