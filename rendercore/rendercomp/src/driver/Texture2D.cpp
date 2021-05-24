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

#include <rendercomp/driver/Texture2D.h>

namespace rendercomp
{
Texture2D::Texture2D(const data::Image* const image,
                     const uint32_t mipLevels) RC_NOEXCEPT
 : Texture2D(image,
             mipLevels,
             TextureUtils::guessTextureFormat(image->channels),
             TextureUtils::guessTextureInternalFormat(image->channels, image->channelDepth),
             TextureUtils::guessTexturePixelFormat(image->channelDepth))
{
}

Texture2D::Texture2D(const data::Image* const image,
                     const uint32_t mipLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
 : _width(image->width)
 , _height(image->height)
{
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("Texture2D: Zero width and/or height");
#endif
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   mipLevels,
                                   static_cast<uint32_t>(internalFormat),
                                   image->width,
                                   image->height));
    DRIVER_CALL(glTextureSubImage2D(_texHandle,
                                    0,
                                    0,
                                    0,
                                    image->width,
                                    image->height,
                                    static_cast<uint32_t>(format),
                                    static_cast<uint32_t>(pixelFormat),
                                    image->pixelData.data()));
    if(mipLevels > 1)
        DRIVER_CALL(glGenerateTextureMipmap(_texHandle));
}

Texture2D::Texture2D(const Vector<const data::Image* const>& mipLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(mipLevels.empty())
        throw std::runtime_error("Texture2D: Empty mip data given");
#endif

    _width = mipLevels[0]->width;
    _height = mipLevels[0]->height;

#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("Texture2D: Zero width and/or height");
#endif

    DRIVER_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   mipLevels.size(),
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height));
    for(size_t i = 0; i < mipLevels.size(); ++i)
    {
        const data::Image* const image = mipLevels[i];
        DRIVER_CALL(glTextureSubImage2D(_texHandle,
                                        i,
                                        0,
                                        0,
                                        image->width,
                                        image->height,
                                        static_cast<uint32_t>(format),
                                        static_cast<uint32_t>(pixelFormat),
                                        image->pixelData.data()));
    }
}

Texture2D::Texture2D(const uint32_t width,
                     const uint32_t height,
                     const TextureInternalFormat internalFormat) RC_NOEXCEPT
 : _width(width)
 , _height(height)
{
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("Texture2D: Zero width and/or height");
#endif
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   1,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height));

}

Texture2D::Texture2D(const uint32_t width,
                     const uint32_t height,
                     const DepthStencilInternalFormat internalFormat) RC_NOEXCEPT
 : _width(width)
 , _height(height)
{
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("Texture2D: Zero width and/or height");
#endif
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   1,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height));
}

Texture2D::Texture2D(Texture2D&& tex) RC_NOEXCEPT
{
    *this = std::move(tex);
}

Texture2D& Texture2D::operator=(Texture2D && tex) RC_NOEXCEPT
{
    std::swap(_texHandle, tex._texHandle);
    _width = std::move(tex._width);
    _height = std::move(tex._height);
    return *this;
}

Texture2D::~Texture2D()
{
    if(_texHandle != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteTextures(1, &_texHandle));
}

void Texture2D::bind(const uint32_t bindIndex) const RC_NOEXCEPT
{
    DRIVER_CALL(glBindTextureUnit(bindIndex, _texHandle));
}

void Texture2D::setDepthStencilMode(const TextureDepthStencilMode mode)  const RC_NOEXCEPT
{
    TextureUtils::setDepthStencilMode(_texHandle, mode);
}

void Texture2D::setCompareMode(const TextureCompareMode mode,
                               const TextureCompareFunc func) const RC_NOEXCEPT
{
    TextureUtils::setCompareMode(_texHandle, mode, func);
}

void Texture2D::setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapS(_texHandle, mode);
}

void Texture2D::setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapT(_texHandle, mode);
}

void Texture2D::setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setBaseMipmapLevel(_texHandle, val);
}

void Texture2D::setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setMaxMipmapLevel(_texHandle, val);
}

void Texture2D::setMinLOD(const float min) const RC_NOEXCEPT
{
    TextureUtils::setMinLOD(_texHandle, min);
}

void Texture2D::setMaxLOD(const float max) const RC_NOEXCEPT
{
    TextureUtils::setMaxLOD(_texHandle, max);
}

void Texture2D::setLODBias(const float val) const RC_NOEXCEPT
{
    TextureUtils::setLODBias(_texHandle, val);
}

void Texture2D::setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMinFilter(_texHandle, filter);
}

void Texture2D::setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMagFilter(_texHandle, filter);
}

}
