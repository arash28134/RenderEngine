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

#include <rendercomp/driver/Texture1D.h>

namespace rendercomp
{
Texture1D::Texture1D(const data::Image* const image,
                     const uint32_t mipmapLevels) RC_NOEXCEPT
 : Texture1D(image,
             mipmapLevels,
             TextureUtils::guessTextureFormat(image->channels),
             TextureUtils::guessTextureInternalFormat(image->channels, image->channelDepth),
             TextureUtils::guessTexturePixelFormat(image->channelDepth))
{
}

Texture1D::Texture1D(const data::Image* const image,
                     const uint32_t mipmapLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
 : _width(image->width)
{
#ifdef RENDERCOMP_DEBUG
    if(image->height != 1)
        throw std::invalid_argument("Texture1D: source image must have a height of 1");
    if(_width == 0)
        throw std::invalid_argument("Texture1D: Zero width");
#endif

    DRIVER_CALL(glCreateTextures(GL_TEXTURE_1D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage1D(_texHandle,
                                   mipmapLevels,
                                   static_cast<uint32_t>(internalFormat),
                                   image->width));
    DRIVER_CALL(glTextureSubImage1D(_texHandle,
                                    0,
                                    0,
                                    image->width,
                                    static_cast<uint32_t>(format),
                                    static_cast<uint32_t>(pixelFormat),
                                    image->pixelData.data()));
    if(mipmapLevels > 1)
        DRIVER_CALL(glGenerateTextureMipmap(_texHandle));
}

Texture1D::Texture1D(const Vector<data::Image* const>& mipLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(mipLevels.empty())
        throw std::runtime_error("Texture1D: Empty mipmap level list given");
#endif

    _width = mipLevels[0]->width;

#ifdef RENDERCOMP_DEBUG
    if(mipLevels[0]->height != 1)
        throw std::invalid_argument("Texture1D: source image must have a height of 1");
    if(_width == 0)
        throw std::invalid_argument("Texture1D: Zero width");
#endif

    DRIVER_CALL(glCreateTextures(GL_TEXTURE_1D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage1D(_texHandle,
                                   mipLevels.size(),
                                   static_cast<uint32_t>(internalFormat),
                                   _width));

    for(size_t i = 0; i < mipLevels.size(); ++i)
    {
        const data::Image* const image = mipLevels[i];
        DRIVER_CALL(glTextureSubImage1D(_texHandle,
                                        i,
                                        0,
                                        image->width,
                                        static_cast<uint32_t>(format),
                                        static_cast<uint32_t>(pixelFormat),
                                        image->pixelData.data()));
    }
}

Texture1D::Texture1D(Texture1D&& tex) RC_NOEXCEPT
{
    *this = std::move(tex);
}

Texture1D& Texture1D::operator=(Texture1D&& tex) RC_NOEXCEPT
{
    std::swap(_texHandle, tex._texHandle);
    _width = std::move(tex._width);
    return *this;
}

Texture1D::~Texture1D()
{
    if(_texHandle != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteTextures(1, &_texHandle));
}

void Texture1D::bind(const uint32_t bindIndex) const RC_NOEXCEPT
{
    DRIVER_CALL(glBindTextureUnit(bindIndex, _texHandle));
}

void Texture1D::setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapS(_texHandle, mode);
}

void Texture1D::setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setBaseMipmapLevel(_texHandle, val);
}

void Texture1D::setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setMaxMipmapLevel(_texHandle, val);
}

void Texture1D::setMinLOD(const float min) const RC_NOEXCEPT
{
    TextureUtils::setMinLOD(_texHandle, min);
}

void Texture1D::setMaxLOD(const float max) const RC_NOEXCEPT
{
    TextureUtils::setMaxLOD(_texHandle, max);
}

void Texture1D::setLODBias(const float val) const RC_NOEXCEPT
{
    TextureUtils::setLODBias(_texHandle, val);
}

void Texture1D::setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMinFilter(_texHandle, filter);
}

void Texture1D::setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMagFilter(_texHandle, filter);
}
}
