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

#include <rendercomp/driver/Texture3D.h>

namespace rendercomp
{
Texture3D::Texture3D(const data::Volume* const volume,
                     const uint32_t mipLevels) RC_NOEXCEPT
 : Texture3D(volume,
             mipLevels,
             TextureUtils::guessTextureFormat(volume->channels),
             TextureUtils::guessTextureInternalFormat(volume->channels, volume->channelDepth),
             TextureUtils::guessTexturePixelFormat(volume->channelDepth))
{
}

Texture3D::Texture3D(const data::Volume* const volume,
                     const uint32_t mipLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
 : _width(volume->width)
 , _height(volume->height)
 , _depth(volume->depth)
{ 
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0 || _depth == 0)
        throw std::invalid_argument("Texture3D: Zero width, height or depth");
#endif

    DRIVER_CALL(glCreateTextures(GL_TEXTURE_3D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage3D(_texHandle,
                                   mipLevels,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height,
                                   _depth));
    DRIVER_CALL(glTextureSubImage3D(_texHandle,
                                    0,
                                    0,
                                    0,
                                    0,
                                    _width,
                                    _height,
                                    _depth,
                                    static_cast<uint32_t>(format),
                                    static_cast<uint32_t>(pixelFormat),
                                    volume->data.data()));

    if(mipLevels > 1)
        DRIVER_CALL(glGenerateTextureMipmap(_texHandle));
}

Texture3D::Texture3D(const Vector<const data::Volume* const>& mipLevels,
                     const TextureFormat format,
                     const TextureInternalFormat internalFormat,
                     const TexturePixelFormat pixelFormat) RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(mipLevels.empty())
        throw std::runtime_error("Texture3D: Empty mip level data");
#endif

    _width = mipLevels[0]->width;
    _height = mipLevels[0]->height;
    _depth = mipLevels[0]->depth;

#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0 || _depth == 0)
        throw std::invalid_argument("Texture3D: Zero width, height or depth");
#endif

    DRIVER_CALL(glCreateTextures(GL_TEXTURE_3D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage3D(_texHandle,
                                   mipLevels.size(),
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height,
                                   _depth));
    for(size_t i = 0; i < mipLevels.size(); ++i)
    {
        const data::Volume* const volume = mipLevels[i];
        DRIVER_CALL(glTextureSubImage3D(_texHandle,
                                        i,
                                        0,
                                        0,
                                        0,
                                        _width,
                                        _height,
                                        _depth,
                                        static_cast<uint32_t>(format),
                                        static_cast<uint32_t>(pixelFormat),
                                        volume->data.data()));
    }
}

Texture3D::Texture3D(const uint32_t width,
                     const uint32_t height,
                     const uint32_t depth,
                     const TextureInternalFormat internalFormat) RC_NOEXCEPT
 : _width(width)
 , _height(height)
 , _depth(depth)
{
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0 || _depth == 0)
        throw std::invalid_argument("Texture3D: Zero width, height or depth");
#endif
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_3D, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage3D(_texHandle,
                                   1,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height,
                                   _depth));
}

Texture3D::Texture3D(Texture3D&& o) RC_NOEXCEPT
{
    *this = std::move(o);
}

Texture3D& Texture3D::operator=(Texture3D&& o) RC_NOEXCEPT
{
    std::swap(_texHandle, o._texHandle);
    _width = std::move(o._width);
    _height = std::move(o._height);
    _depth = std::move(o._depth);

    return *this;
}

Texture3D::~Texture3D()
{
    if(_texHandle != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteTextures(1, &_texHandle));
}

void Texture3D::bind(const uint32_t bindIndex) const RC_NOEXCEPT
{
    DRIVER_CALL(glBindTextureUnit(bindIndex, _texHandle));
}

void Texture3D::setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapS(_texHandle, mode);
}

void Texture3D::setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapT(_texHandle, mode);
}

void Texture3D::setWrapR(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapR(_texHandle, mode);
}

void Texture3D::setBaseMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setBaseMipmapLevel(_texHandle, val);
}

void Texture3D::setMaxMipmapLevel(const uint32_t val) const RC_NOEXCEPT
{
    TextureUtils::setMaxMipmapLevel(_texHandle, val);
}

void Texture3D::setMinLOD(const float min) const RC_NOEXCEPT
{
    TextureUtils::setMinLOD(_texHandle, min);
}

void Texture3D::setMaxLOD(const float max) const RC_NOEXCEPT
{
    TextureUtils::setMaxLOD(_texHandle, max);
}

void Texture3D::setLODBias(const float val) const RC_NOEXCEPT
{
    TextureUtils::setLODBias(_texHandle, val);
}

void Texture3D::setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMinFilter(_texHandle, filter);
}

void Texture3D::setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMagFilter(_texHandle, filter);
}
}
