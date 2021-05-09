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


#include <rendercomp/core/BoundingBox.h>
#include <rendercomp/core/math/Intersection.h>

namespace rendercomp
{
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
 : _min(Vec3f(9999999.9f, 9999999.9f, 9999999.9f))
 , _max(Vec3f(-9999999.9f, -9999999.9f, -9999999.9f))
{
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vec3f& min, const Vec3f& max)
 : _min(min)
 , _max(max)
{
}

Vector<Vec3f> AxisAlignedBoundingBox::getBoundPoints() const
{
    return Vector<Vec3f> {
            _min,
            Vec3f(_min.x, _min.y, _max.z),
            Vec3f(_max.x, _min.y, _min.z),
            Vec3f(_max.x, _min.y, _max.z),
            Vec3f(_min.x, _max.y, _min.z),
            Vec3f(_min.x, _max.y, _max.z),
            Vec3f(_max.x, _max.y, _min.z),
            _max
    };
}

template<>
void AxisAlignedBoundingBox::expand(const Vec3f& point)
{
    _min = glm::min(_min, point);
    _max = glm::max(_max, point);
}

template<>
void AxisAlignedBoundingBox::expand(const AxisAlignedBoundingBox& aabb)
{
    _min = glm::min(_min, aabb._min);
    _max = glm::max(_max, aabb._max);
}

template<>
bool AxisAlignedBoundingBox::intersect(const Vec3f& point) const
{
    return PointIntersectAABB(_min, _max, point);
}

template<>
bool AxisAlignedBoundingBox::intersect(const AxisAlignedBoundingBox& aabb) const
{
    return AABBIntersectAABB(_min, _max, aabb._min, aabb._max);
}
}
