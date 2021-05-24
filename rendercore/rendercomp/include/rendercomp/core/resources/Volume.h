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

#include "../../common/Types.h"

namespace rendercomp
{
namespace data
{
struct Volume
{
    // Width of the volume, in voxels
    uint32_t width;
    // Height of the volume, in voxels
    uint32_t height;
    // Depth of the volume, in voxels
    uint32_t depth;
    // How many channels per voxel
    uint8_t channels;
    // How many bytes per channel
    uint8_t channelDepth;
    // Data as a byte array
    Vector<uint8_t> data;
};
}
}
