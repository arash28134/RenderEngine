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
float Cotangent(const Vec3f& pivot, const Vec3f& a, const Vec3f& b);
Vec3f VoronoiTriangleAreas(const Vec3f& A, const Vec3f& B, const Vec3f& C);
Vector<Vec3f> ComputeSmoothNormals(const Vector<Vec3ui>& indices, const Vector<Vec3f>& vertices);
}
