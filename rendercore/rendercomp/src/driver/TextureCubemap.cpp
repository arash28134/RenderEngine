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

#include <rendercomp/driver/TextureCubemap.h>

namespace rendercomp
{

namespace
{
TextureFormat
guessFormat(const CubemapImages& faces) RC_NOEXCEPT
{
    if(!faces.right)
    {
#ifdef RENDERCOMP_DEBUG
        throw std::runtime_error("TextureCubemap: NULL Face images");
#endif
        return TextureFormat::INVALID;
    }

    return TextureUtils::guessTextureFormat(faces.right->channels);
}

TextureInternalFormat
guessInternalFormat(const CubemapImages& faces) RC_NOEXCEPT
{
    if(!faces.right)
    {
#ifdef RENDERCOMP_DEBUG
        throw std::runtime_error("TextureCubemap: NULL Face images");
#endif
        return TextureInternalFormat::INVALID;
    }

    return TextureUtils::guessTextureInternalFormat(faces.right->channels,
                                                    faces.right->channelDepth);
}

TexturePixelFormat
guessPixelFormat(const CubemapImages& faces) RC_NOEXCEPT
{
    if(!faces.right)
    {
#ifdef RENDERCOMP_DEBUG
        throw std::runtime_error("TextureCubemap: NULL Face images");
#endif
        return TexturePixelFormat::INVALID;
    }

    return TextureUtils::guessTexturePixelFormat(faces.right->channelDepth);
}

}

TextureCubemap::TextureCubemap(const CubemapImages& faces) RC_NOEXCEPT
 : TextureCubemap(faces,
                  guessFormat(faces),
                  guessInternalFormat(faces),
                  guessPixelFormat(faces))
{
}

TextureCubemap::TextureCubemap(const CubemapImages& faces,
                               const TextureFormat format,
                               const TextureInternalFormat internalFormat,
                               const TexturePixelFormat pixelFormat) RC_NOEXCEPT
{
#ifdef RENDERCOMP_DEBUG
    if(!faces.right || !faces.left || !faces.top || !faces.bottom || !faces.front || !faces.back)
        throw std::invalid_argument("TextureCubemap: NULL Face images");
#endif
    _width = faces.right->width;
    _height = faces.right->height;
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("TextureCubemap: Zero width and/or height");
#endif
    // reddit.com/r/opengl/comments/556zac/how_to_create_cubemap_with_direct_state_access
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   1,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height));

    const Vector<const data::Image* const> facesV =
    {
        faces.right,
        faces.left,
        faces.bottom,
        faces.top,
        faces.front,
        faces.back
    };
    for(size_t i = 0; i < 6; ++i)
    {
        const data::Image* const image = facesV[i];
        DRIVER_CALL(glTextureSubImage3D(_texHandle,
                                        0,
                                        0,
                                        0,
                                        i, // face
                                        _width,
                                        _height,
                                        1, // depth
                                        static_cast<uint32_t>(format),
                                        static_cast<uint32_t>(pixelFormat),
                                        image->pixelData.data()));
    }
}

TextureCubemap::TextureCubemap(const uint32_t width,
                               const uint32_t height,
                               const TextureInternalFormat internalFormat) RC_NOEXCEPT
 : _width(width)
 , _height(height)
{
#ifdef RENDERCOMP_DEBUG
    if(_width == 0 || _height == 0)
        throw std::invalid_argument("TextureCubemap: Zero width and/or height");
#endif
    DRIVER_CALL(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_texHandle));
    DRIVER_CALL(glTextureStorage2D(_texHandle,
                                   1,
                                   static_cast<uint32_t>(internalFormat),
                                   _width,
                                   _height));

}

TextureCubemap::TextureCubemap(TextureCubemap&& tex) RC_NOEXCEPT
{
    *this = std::move(tex);
}

TextureCubemap& TextureCubemap::operator=(TextureCubemap && tex) RC_NOEXCEPT
{
    std::swap(_texHandle, tex._texHandle);
    _width = std::move(tex._width);
    _height = std::move(tex._height);
    return *this;
}

TextureCubemap::~TextureCubemap()
{
    if(_texHandle != GL_INVALID_VALUE)
        DRIVER_CALL_NOTHROW(glDeleteTextures(1, &_texHandle));
}

void TextureCubemap::bind(const uint32_t bindIndex) const RC_NOEXCEPT
{
    DRIVER_CALL(glBindTextureUnit(bindIndex, _texHandle));
}

void TextureCubemap::setWrapS(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapS(_texHandle, mode);
}

void TextureCubemap::setWrapT(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapT(_texHandle, mode);
}

void TextureCubemap::setWrapR(const TextureWrapMode mode) const RC_NOEXCEPT
{
    TextureUtils::setWrapR(_texHandle, mode);
}

void TextureCubemap::setMinFilter(const TextureMinFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMinFilter(_texHandle, filter);
}

void TextureCubemap::setMagFilter(const TextureMagFilter filter) const RC_NOEXCEPT
{
    TextureUtils::setMagFilter(_texHandle, filter);
}
}
