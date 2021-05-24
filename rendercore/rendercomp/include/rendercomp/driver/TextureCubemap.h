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
struct CubemapImages
{
    data::Image* right {nullptr};
    data::Image* left {nullptr};
    data::Image* top {nullptr};
    data::Image* bottom {nullptr};
    data::Image* front {nullptr};
    data::Image* back {nullptr};
};

class TextureCubemap
{
public:
    TextureCubemap(const CubemapImages& faces) RC_NOEXCEPT;
    TextureCubemap(const CubemapImages& faces,
                   const TextureFormat format,
                   const TextureInternalFormat internalFormat,
                   const TexturePixelFormat pixelFormat) RC_NOEXCEPT;
    TextureCubemap(const uint32_t width,
                   const uint32_t height,
                   const TextureInternalFormat internalFormat) RC_NOEXCEPT;

    TextureCubemap(const TextureCubemap&) = delete;
    TextureCubemap& operator=(const TextureCubemap&) = delete;

    TextureCubemap(TextureCubemap&&) RC_NOEXCEPT;
    TextureCubemap& operator=(TextureCubemap&&) RC_NOEXCEPT;

    ~TextureCubemap();

    void bind(const uint32_t bindIndex) const RC_NOEXCEPT;
    void setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setWrapR(const TextureWrapMode mode) const RC_NOEXCEPT;
    void setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT;
    void setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT;

private:
    uint32_t _texHandle;
    uint32_t _width, _height;
};
}
