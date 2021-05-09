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

#include <memory>

#include "../Resources.h"

#include <iostream>

namespace rendercomp
{
namespace data
{
class Mesh
{
public:
    Vector<Vec3f> vertexPositions;
    Vector<Vec3f> vertexNormals;
    Vector<Vec3f> vertexTangents;
    Vector<Vector<Vec2f>> vertexUVs;
    Vector<Vector<Vec4f>> vertexColors;
    Vector<Vec3ui> faces;
};

}
template<>
std::unique_ptr<data::Mesh>
Resources::get<data::Mesh>(const String& path);
}
