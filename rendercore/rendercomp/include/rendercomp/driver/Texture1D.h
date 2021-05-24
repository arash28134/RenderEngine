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

#include "../core/resources/Image.h"

#include "DriverDebug.h"
#include "TextureUtils.h"

namespace rendercomp
{
class Texture1D
{
public:
    Texture1D(const data::Image* const image, const uint32_t mimapLevels) RC_NOEXCEPT;
    Texture1D(const data::Image* const image,
              const uint32_t mipmapLevels,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;
    Texture1D(const Vector<data::Image* const>& mipLevels,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;

    Texture1D(const Texture1D&) = delete;
    Texture1D& operator=(const Texture1D&) = delete;

    Texture1D(Texture1D&&) RC_NOEXCEPT;
    Texture1D& operator=(Texture1D&&) RC_NOEXCEPT;

    ~Texture1D();

    void bind(const uint32_t bindIndex) const RC_NOEXCEPT;
    void setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMinLOD(const float min) const RC_NOEXCEPT;
    void setMaxLOD(const float max) const RC_NOEXCEPT;
    void setLODBias(const float val) const RC_NOEXCEPT;
    void setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT;
    void setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT;

private:
    Texture1D() = default;

    GLuint _texHandle {GL_INVALID_VALUE};
    uint32_t _width;
};
}
