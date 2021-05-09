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

#include <stdlib.h>

#include <rendercomp/common/Types.h>

// Dummy implementations for EASTL required definitions of operator new until a propper
// allocator for this engine is written

void* operator new[](size_t size,
                     const char* /*pName*/,
                     int /*flags*/,
                     unsigned /*debugFlags*/,
                     const char* /*file*/,
                     int /*line*/)
{
    return malloc(size);
}

void* operator new[](size_t size,
                     size_t /*alignment*/,
                     size_t /*alignmentOffset*/,
                     const char* /*pName*/,
                     int /*flags*/,
                     unsigned /*debugFlags*/,
                     const char* /*file*/,
                     int /*line*/)
{
    return malloc(size);
}

std::ostream& operator<<(std::ostream& os, const rendercomp::String& str)
{
    return os << str.c_str();
}
