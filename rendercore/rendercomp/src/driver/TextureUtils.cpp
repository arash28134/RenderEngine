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

#include <rendercomp/driver/TextureUtils.h>

namespace rendercomp
{
TextureFormat TextureUtils::guessTextureFormat(const uint8_t numChannels) RC_NOEXCEPT
{
    switch(numChannels)
    {
    case 1:
        return TextureFormat::R;
    case 2:
        return TextureFormat::RG;
    case 3:
        return TextureFormat::RGB;
    case 4:
        return TextureFormat::RGBA;
    }

#ifdef RENDECOMP_DEBUG
    throw std::invalid_argument("Invalid number of channels: " + std::to_string(numChannels));
#else
    return TextureFormat::INVALID;
#endif
}

TextureInternalFormat
TextureUtils::guessTextureInternalFormat(const uint8_t numChannels,
                                         const uint8_t bytesPerChannel) RC_NOEXCEPT
{
    switch(numChannels)
    {
    case 1:
        switch(bytesPerChannel)
        {
        case 1:
            return TextureInternalFormat::R8;
        case 2:
            return TextureInternalFormat::R_16F;
        case 4:
            return TextureInternalFormat::R_32F;
        default:
#ifdef RENDERCOMP_DEBUG
            throw std::invalid_argument("Invalid bytes per channel: "
                                        + std::to_string(bytesPerChannel));
#else
            return TextureInternalFormat::INVALID;
#endif
        }
        break;
    case 2:
        switch(bytesPerChannel)
        {
        case 1:
            return TextureInternalFormat::RG8;
        case 2:
            return TextureInternalFormat::RG_16F;
        case 4:
            return TextureInternalFormat::RG_32F;
        default:
#ifdef RENDERCOMP_DEBUG
            throw std::invalid_argument("Invalid bytes per channel: "
                                        + std::to_string(bytesPerChannel));
#else
            return TextureInternalFormat::INVALID;
#endif
        }
        break;
    case 3:
        switch(bytesPerChannel)
        {
        case 1:
            return TextureInternalFormat::RGB8;
        case 2:
            return TextureInternalFormat::RGB_16F;
        case 4:
            return TextureInternalFormat::RGB_32F;
        default:
#ifdef RENDERCOMP_DEBUG
            throw std::invalid_argument("Invalid bytes per channel: "
                                        + std::to_string(bytesPerChannel));
#else
            return TextureInternalFormat::INVALID;
#endif
        }
        break;
    case 4:
        switch(bytesPerChannel)
        {
        case 1:
            return TextureInternalFormat::RGBA8;
        case 2:
            return TextureInternalFormat::RGBA_16F;
        case 4:
            return TextureInternalFormat::RGBA_32F;
        default:
#ifdef RENDERCOMP_DEBUG
            throw std::invalid_argument("Invalid bytes per channel: "
                                        + std::to_string(bytesPerChannel));
#else
            return TextureInternalFormat::INVALID;
#endif
        }
        break;
    }

#ifdef RENDERCOMP_DEBUG
    throw std::invalid_argument("Invalid number of channels: " + std::to_string(numChannels));
#else
    return TextureInternalFormat::INVALID;
#endif
}

TexturePixelFormat TextureUtils::guessTexturePixelFormat(const uint8_t bytesPerChannel) RC_NOEXCEPT
{
    switch(bytesPerChannel)
    {
    case 1:
        return TexturePixelFormat::UNSIGNED_BYTE;
    case 2:
        return TexturePixelFormat::UNSIGNED_SHORT;
   case 4:
        return TexturePixelFormat::UNSIGNED_INT;
    }

#ifdef RENDERCOMP_DEBUG
    throw std::invalid_argument("Invalid bytes per channel: "
                                + std::to_string(bytesPerChannel));
#else
    return TexturePixelFormat::INVALID;
#endif
}

void TextureUtils::setDepthStencilMode(const uint32_t texture,
                                       const TextureDepthStencilMode mode) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_DEPTH_STENCIL_TEXTURE_MODE,
                                    static_cast<uint32_t>(mode)));
}

void TextureUtils::setCompareMode(const uint32_t texture,
                                  const TextureCompareMode mode,
                                  const TextureCompareFunc func) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_COMPARE_MODE,
                                    static_cast<uint32_t>(mode)));
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_COMPARE_FUNC,
                                    static_cast<uint32_t>(func)));
}

void TextureUtils::setWrapS(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_WRAP_S,
                                    static_cast<uint32_t>(mode)));
}

void TextureUtils::setWrapT(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_WRAP_T,
                                    static_cast<uint32_t>(mode)));
}

void TextureUtils::setWrapR(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_WRAP_R,
                                    static_cast<uint32_t>(mode)));
}

void TextureUtils::setBaseMipmapLevel(const uint32_t texture, const uint32_t val) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, val));
}

void TextureUtils::setBorderF(const uint32_t texture, const Vec4f& color) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, &color[0]));
}

void TextureUtils::setBorderI(const uint32_t texture, const Vec4i& color) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterIiv(texture, GL_TEXTURE_BORDER_COLOR, &color[0]));
}

void TextureUtils::setBorderUI(const uint32_t texture, const Vec4ui& color) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterIuiv(texture, GL_TEXTURE_BORDER_COLOR, &color[0]));
}

void TextureUtils::setMinLOD(const uint32_t texture, const float min) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterf(texture, GL_TEXTURE_MIN_LOD, min));
}

void TextureUtils::setMaxLOD(const uint32_t texture, const float max) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterf(texture, GL_TEXTURE_MAX_LOD, max));
}

void TextureUtils::setLODBias(const uint32_t texture, const float val) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameterf(texture, GL_TEXTURE_LOD_BIAS, val));
}

void TextureUtils::setMaxMipmapLevel(const uint32_t texture, const uint32_t val) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, val));
}

void TextureUtils::setMinFilter(const uint32_t texture, const TextureMinFilter filter) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_MIN_FILTER,
                                    static_cast<uint32_t>(filter)));
}

void TextureUtils::setMagFilter(const uint32_t texture, const TextureMagFilter filter) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_MAG_FILTER,
                                    static_cast<uint32_t>(filter)));
}

void TextureUtils::setSwizzleR(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_SWIZZLE_R,
                                    static_cast<uint32_t>(C)));
}

void TextureUtils::setSwizzleG(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_SWIZZLE_G,
                                    static_cast<uint32_t>(C)));
}

void TextureUtils::setSwizzleB(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_SWIZZLE_B,
                                    static_cast<uint32_t>(C)));
}

void TextureUtils::setSwizzleA(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT
{
    DRIVER_CALL(glTextureParameteri(texture,
                                    GL_TEXTURE_SWIZZLE_A,
                                    static_cast<uint32_t>(C)));
}

void TextureUtils::setSwizzleRGBA(const uint32_t texture,
                                  const TextureSwizzleChannel R,
                                  const TextureSwizzleChannel G,
                                  const TextureSwizzleChannel B,
                                  const TextureSwizzleChannel A) RC_NOEXCEPT
{
    const Vector<int32_t> swizzleValues =
    {
        static_cast<int32_t>(R),
        static_cast<int32_t>(G),
        static_cast<int32_t>(B),
        static_cast<int32_t>(A)
    };
    DRIVER_CALL(glTextureParameteriv(texture, GL_TEXTURE_SWIZZLE_RGBA, swizzleValues.data()));
}
}
