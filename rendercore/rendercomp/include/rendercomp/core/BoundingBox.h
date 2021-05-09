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

#include "../common/Types.h"

namespace rendercomp
{
class AxisAlignedBoundingBox
{
public:
    AxisAlignedBoundingBox();
    AxisAlignedBoundingBox(const Vec3f& min, const Vec3f& max);

    AxisAlignedBoundingBox(const AxisAlignedBoundingBox&) = default;
    AxisAlignedBoundingBox(AxisAlignedBoundingBox&&) = default;

    ~AxisAlignedBoundingBox() = default;

    AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox&) = default;
    AxisAlignedBoundingBox& operator=(AxisAlignedBoundingBox&&) = default;

    template<typename T>
    void expand(T&&)
    {
        throw std::runtime_error("AxisAlignedBoundingBox: No expand implementation for type "
                                 + std::string(typeid(T).name()));
    }

    template<typename T>
    bool intersect(T&&) const
    {
        throw std::runtime_error("AxisAlignedBoundingBox: No intersect implementation for type "
                                 + std::string(typeid(T).name()));
    }

    Vector<Vec3f> getBoundPoints() const;

    const Vec3f& getMin() const
    {
        return _min;
    }

    const Vec3f& getMax() const
    {
        return _max;
    }

    Vec3f center() const
    {
        return (_min + _max) * 0.5f;
    }

    float getXLength() const
    {
        return (_max.x - _min.x);
    }

    float getYLength() const
    {
        return (_max.y - _min.y);
    }

    float getZLength() const
    {
        return (_max.z - _min.z);
    }

    void setMin(const Vec3f& minV)
    {
        _min = minV;
    }

    void setMax(const Vec3f& maxV)
    {
        _max = maxV;
    }

    void translate(const Vec3f& delta)
    {
        _min += delta;
        _max += delta;
    }

private:
    Vec3f _min, _max;
};

template<>
void AxisAlignedBoundingBox::expand(const Vec3f& point);

template<>
void AxisAlignedBoundingBox::expand(const AxisAlignedBoundingBox& aabb);

template<>
bool AxisAlignedBoundingBox::intersect(const Vec3f& point) const;

template<>
bool AxisAlignedBoundingBox::intersect(const AxisAlignedBoundingBox& aabb) const;

}
