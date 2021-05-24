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

#include "../core/resources/Volume.h"

#include "DriverDebug.h"
#include "TextureUtils.h"

namespace rendercomp
{
class Texture3D
{
public:
    Texture3D(const data::Volume* const volume,
              const uint32_t mipLevels) RC_NOEXCEPT;

    Texture3D(const data::Volume* const volume,
              const uint32_t mipLevels,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;

    Texture3D(const Vector<const data::Volume* const>& mipData,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;

    Texture3D(const uint32_t width,
              const uint32_t height,
              const uint32_t depth,
              const TextureInternalFormat internalFormat) RC_NOEXCEPT;

    Texture3D(const Texture3D&) = delete;
    Texture3D& operator=(const Texture3D&) = delete;

    Texture3D(Texture3D&&) RC_NOEXCEPT;
    Texture3D& operator=(Texture3D&&) RC_NOEXCEPT;

    ~Texture3D();

    void bind(const uint32_t bindIndex) const RC_NOEXCEPT;
    void setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setWrapR(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMinLOD(const float min) const RC_NOEXCEPT;
    void setMaxLOD(const float max) const RC_NOEXCEPT;
    void setLODBias(const float val) const RC_NOEXCEPT;
    void setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT;
    void setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT;

private:
    Texture3D() = default;

    uint32_t _texHandle {GL_INVALID_VALUE};
    uint32_t _width, _height, _depth;
};
}
