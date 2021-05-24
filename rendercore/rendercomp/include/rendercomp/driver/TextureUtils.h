/*
 * This file is part of the RenderEngine distribution (const uint32_t texture, https://github.com/NadirRoGue/RenderEngine).
 * Copyright (const uint32_t texture, c) 2021 Nadir Román Guerrero.
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

#include "../common/Types.h"
#include "../core/resources/Image.h"

#include "DriverDebug.h"

#include <glad/glad.h>

namespace rendercomp
{

enum class TextureMinFilter : uint32_t
{
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter : uint32_t
{
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR
};

enum class TextureSwizzleChannel : uint32_t
{
    RED = GL_RED,
    GREEN = GL_GREEN,
    BLUE = GL_BLUE,
    ALPHA = GL_ALPHA,
    ZERO = GL_ZERO,
    ONE = GL_ONE
};

enum class TextureDepthStencilMode : uint32_t
{
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    STENCIL_INDEX = GL_STENCIL_INDEX
};

enum class TextureCompareMode : uint32_t
{
    TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
    TO_NONE = GL_NONE
};

enum class TextureCompareFunc : uint32_t
{
    LESS_EQUAL = GL_LEQUAL,
    GREATER_EQUAL = GL_GEQUAL,
    LESS = GL_LESS,
    GREATER = GL_GREATER,
    EQUAL = GL_EQUAL,
    NOT_EQUAL = GL_NOTEQUAL,
    CONSTANT_1 = GL_ALWAYS,
    CONSTANT_0 = GL_NEVER
};

enum class TextureWrapMode : uint32_t
{
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    MIRROR_REPEAT = GL_MIRRORED_REPEAT,
    REPEAT = GL_REPEAT,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
};

enum class TextureFormat : uint32_t
{
    R = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    BGR = GL_BGR,
    RGBA = GL_RGBA,
    BGRA = GL_BGRA,
    INVALID = GL_INVALID_ENUM
};

enum class DepthStencilFormat : uint32_t
{
    DEPTH_DATA = GL_DEPTH_COMPONENT,
    STENCIL_DATA = GL_STENCIL_INDEX,
};

enum class TextureInternalFormat : uint32_t
{
    /**
     *  Single channel 8 bit sampled as float [0.0, 1.0]
     */
    R8 = GL_R8,
    /**
     *  2-channel, 8 bit each, sampled as float [0.0, 1.0]
     */
    RG8 = GL_RG8,
    /**
     *  3-channel, 8 bit each, sampled as float [0.0, 1.0]
     */
    RGB8 = GL_RGB8,
    /**
     *  4-channel, 8 bit each, sampled as float [0.0, 1.0]
     */
    RGBA8 = GL_RGBA8,

    /**
     *  Single channel 16 bit sampled as float [0.0, 1.0]
     */
    R16 = GL_R16,
    /**
     *  2-channel, 16 bit each, sampled as float [0.0, 1.0]
     */
    RG16 = GL_RG16,
    /**
     *  3-channel, 16 bit each, sampled as float [0.0, 1.0]
     */
    RGB16 = GL_RGB16,
    /**
     * 4-channel, 16 bit each, sampled as float [1.0, 1.0]
     */
    RGBA_16 = GL_RGBA16,

    /**
     *  Single channel 16 bit, sampled as float [-max half float, +max half float]
     */
    R_16F = GL_R16F,
    /**
     *  2-channel, 16 bit each, sampled as float [-max half float, +max half float]
     */
    RG_16F = GL_RG16F,
    /**
     *  3-channel, 16 bit each, sampled as float [-max half float, +max half float]
     */
    RGB_16F = GL_RGB16F,
    /**
     * 4-channel, 16 bit each, sampled as float [-max half float, +max half float]
     */
    RGBA_16F = GL_RGBA16F,

    /**
     *  Single channel 32 bit sampled as float [-max float, +max float]
     */
    R_32F = GL_R32F,
    /**
     *  2-channel, 32 bit each, sampled as float [-max float, +max float]
     */
    RG_32F = GL_RG32F,
    /**
     *  3-channel, 32 bit each, sampled as float [-max float, +max float]
     */
    RGB_32F = GL_RGB32F,
    /**
     *  4-channel, 32 bit each, sampled as float [-max float, +max float]
     */
    RGBA_32F = GL_RGB32F,

    /**
     * Single channel 8 bit sampled as float [-1.0, 1.0]
     */
    SIGNED_R_8 = GL_R8_SNORM,
    /**
     * 2-channel, 8 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RG_8 = GL_RG8_SNORM,
    /**
     * 3-channel, 8 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RGB_8 = GL_RGB8_SNORM,
    /**
     * 4-channel, 8 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RGBA_8 = GL_RGBA8_SNORM,

    /**
     * Single channel 16 bit sampled as float [-1.0, 1.0]
     */
    SIGNED_R_16 = GL_R16_SNORM,
    /**
     * 2-channel, 16 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RG_16 = GL_RG16_SNORM,
    /**
     * 3-channel, 16 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RGB_16 = GL_RGB16_SNORM,
    /**
     * 4-channel, 16 bit each, sampled as float [-1.0, 1.0]
     */
    SIGNED_RGBA_16 = GL_RGBA16_SNORM,

    /**
     * 3-channel sampled as linear color space [0.0, 1.0]
     */
    SRGB_8 = GL_SRGB8,
    /**
     * 4-channel sampled as linear color space [0.0, 1.0]
     */
    SRGBA_8 = GL_SRGB8_ALPHA8,

    /**
     * Represents an invalid internal format
     */
    INVALID = GL_INVALID_ENUM
};

enum class DepthStencilInternalFormat : uint32_t
{
    DEPTH_16 = GL_DEPTH_COMPONENT16,
    DEPTH_24 = GL_DEPTH_COMPONENT24,
    DEPTH_32 = GL_DEPTH_COMPONENT32,
    DEPTH_32F = GL_DEPTH_COMPONENT32F,
    DEPTH_32F_STENCIL_8 = GL_DEPTH32F_STENCIL8,
    DEPTH_24_STENCIL_8 = GL_DEPTH24_STENCIL8
};

enum class TexturePixelFormat : uint32_t
{
    BYTE = GL_BYTE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SHORT = GL_SHORT,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    INT = GL_INT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    FLOAT = GL_FLOAT,
    INVALID = GL_INVALID_ENUM
};

class TextureUtils
{
public:
    static TextureFormat guessTextureFormat(const uint8_t numChannels) RC_NOEXCEPT;
    static TextureInternalFormat
    guessTextureInternalFormat(const uint8_t numChannels,
                               const uint8_t bytesPerChannel) RC_NOEXCEPT;
    static TexturePixelFormat guessTexturePixelFormat(const uint8_t bytesPerChannel) RC_NOEXCEPT;

    static void setDepthStencilMode(const uint32_t texture,
                                    const TextureDepthStencilMode mode) RC_NOEXCEPT;
    static void setCompareMode(const uint32_t texture,
                               const TextureCompareMode mode,
                               const TextureCompareFunc func) RC_NOEXCEPT;
    static void setWrapS(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT;
    static void setWrapT(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT;
    static void setWrapR(const uint32_t texture, const TextureWrapMode mode) RC_NOEXCEPT;
    static void setBaseMipmapLevel(const uint32_t texture, const uint32_t val) RC_NOEXCEPT;
    static void setMaxMipmapLevel(const uint32_t texture, const uint32_t val) RC_NOEXCEPT;
    static void setBorderF(const uint32_t texture, const Vec4f& color) RC_NOEXCEPT;
    static void setBorderI(const uint32_t texture, const Vec4i& color) RC_NOEXCEPT;
    static void setBorderUI(const uint32_t texture, const Vec4ui& color) RC_NOEXCEPT;
    static void setMinLOD(const uint32_t texture, const float min) RC_NOEXCEPT;
    static void setMaxLOD(const uint32_t texture, const float max) RC_NOEXCEPT;
    static void setLODBias(const uint32_t texture, const float val) RC_NOEXCEPT;
    static void setMinFilter(const uint32_t texture, const TextureMinFilter filter) RC_NOEXCEPT;
    static void setMagFilter(const uint32_t texture, const TextureMagFilter filter) RC_NOEXCEPT;
    static void setSwizzleR(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT;
    static void setSwizzleG(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT;
    static void setSwizzleB(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT;
    static void setSwizzleA(const uint32_t texture, const TextureSwizzleChannel C) RC_NOEXCEPT;
    static void setSwizzleRGBA(const uint32_t texture,
                               const TextureSwizzleChannel R,
                               const TextureSwizzleChannel G,
                               const TextureSwizzleChannel B,
                               const TextureSwizzleChannel A) RC_NOEXCEPT;
};
}
