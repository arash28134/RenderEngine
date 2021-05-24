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
class Texture2D
{
public:
    Texture2D(const data::Image* const image,
              const uint32_t mipLevels) RC_NOEXCEPT;

    Texture2D(const data::Image* const image,
              const uint32_t mipLevels,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;

    Texture2D(const Vector<const data::Image* const>& mipLevels,
              const TextureFormat format,
              const TextureInternalFormat internalFormat,
              const TexturePixelFormat pixelFormat) RC_NOEXCEPT;

    Texture2D(const uint32_t width,
              const uint32_t height,
              const TextureInternalFormat internalFormat) RC_NOEXCEPT;

    Texture2D(const uint32_t width,
              const uint32_t height,
              const DepthStencilInternalFormat internalFormat) RC_NOEXCEPT;

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) RC_NOEXCEPT;
    Texture2D& operator=(Texture2D&&) RC_NOEXCEPT;

    ~Texture2D();

    void bind(const uint32_t bindIndex) const RC_NOEXCEPT;
    void setDepthStencilMode(const TextureDepthStencilMode mode) const RC_NOEXCEPT;
    void setCompareMode(const TextureCompareMode mode,
                        const TextureCompareFunc func) const RC_NOEXCEPT;
    void setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT;
    void setMinLOD(const float min) const RC_NOEXCEPT;
    void setMaxLOD(const float max) const RC_NOEXCEPT;
    void setLODBias(const float val) const RC_NOEXCEPT;
    void setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT;
    void setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT;

private:
    Texture2D() = default;

    uint32_t _texHandle {GL_INVALID_VALUE};
    uint32_t _width, _height;
};
}
