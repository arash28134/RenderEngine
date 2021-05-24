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

#include <rendercomp/core/resources/Image.h>

#include <FreeImage.h>

namespace rendercomp
{
namespace
{
//https://github.com/BlueBrain/Brayns/blob/master/brayns/common/utils/imageUtils.cpp
template <class T>
inline void INPLACESWAP(T& a, T& b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

void SwapRedBlue32(FIBITMAP* freeImage)
{
    if (FreeImage_GetImageType(freeImage) != FIT_BITMAP)
        return;

    const uint32_t bytesperpixel = FreeImage_GetBPP(freeImage) / 8;
    if (bytesperpixel > 4 || bytesperpixel < 3)
        return;

    const uint32_t height = FreeImage_GetHeight(freeImage);
    const uint32_t pitch = FreeImage_GetPitch(freeImage);
    const uint32_t lineSize = FreeImage_GetLine(freeImage);

    BYTE* line = FreeImage_GetBits(freeImage);
    for (size_t y = 0; y < height; ++y, line += pitch)
    {
        for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel)
            INPLACESWAP(pixel[0], pixel[2]);
    }
}
}

template<>
std::unique_ptr<data::Image> Resources::get<data::Image>(const String& path)
{
    // Query the image file format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str(), 0);
    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(path.c_str());
    if(fif == FIF_UNKNOWN)
        throw std::runtime_error("Image reader: Could not identify the image format for "
                                 + TOSTDSTR(path));

    if(!FreeImage_FIFSupportsReading(fif))
        throw std::runtime_error("Image reader: No support to read the image format of "
                                 + TOSTDSTR(path));

    // Load the image data
    FIBITMAP* bitmap = FreeImage_Load(fif, path.c_str());
    SwapRedBlue32(bitmap);

    // Query dimensions
    const uint32_t width = FreeImage_GetWidth(bitmap);
    const uint32_t height = FreeImage_GetHeight(bitmap);
    if(width == 0 || height == 0)
        throw std::runtime_error("Image reader: Could not determine dimensions of "
                                 + TOSTDSTR(path));

    uint8_t depth = 1;
    switch (FreeImage_GetImageType(bitmap))
    {
        case FIT_BITMAP:
            depth = 1;
            break;
        case FIT_UINT16:
        case FIT_INT16:
        case FIT_RGB16:
            depth = 2;
            break;
        case FIT_UINT32:
        case FIT_INT32:
        case FIT_RGBA16:
        case FIT_FLOAT:
        case FIT_RGBF:
        case FIT_RGBAF:
            depth = 4;
            break;
        case FIT_DOUBLE:
        case FIT_COMPLEX:
            depth = 8;
            break;
        default:
            throw std::runtime_error("Image reader: Could not determine pixel depth for "
                                     + TOSTDSTR(path));
    }

    const uint8_t bytesPerPixel = FreeImage_GetBPP(bitmap) / 8u;
    const uint8_t channels = bytesPerPixel / depth;

    // Retrieve the pixel data
    BYTE* data = FreeImage_GetBits(bitmap);
    if(!data)
        throw std::runtime_error("Image reader: Could not retrieve the image data of "
                                 + TOSTDSTR(path));

    const uint32_t flatSize = width * height * bytesPerPixel;

    std::unique_ptr<data::Image> result = std::make_unique<data::Image>();
    result->width = width;
    result->height = height;
    result->channels = channels;
    result->channelDepth = depth;
    result->pixelData.insert(result->pixelData.end(), data, data + flatSize);

    FreeImage_Unload(bitmap);

    return result;
}

}
