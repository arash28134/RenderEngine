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
class Transform
{
public:
    Transform();

    Transform(const Transform&) = default;
    Transform(Transform&&) = default;

    Transform& operator=(const Transform&) = default;
    Transform& operator=(Transform&&) = default;

    ~Transform() = default;

    void translate(const Vec3f& delta);
    void setTranslation(const Vec3f& translation);

    void rotateX(const float degree);
    void rotateY(const float degree);
    void rotateZ(const float degree);
    void rotate(const Vec3f& angleAxis);
    void rotate(const Quaterion& quat);
    void setRotation(const Vec3f& angleAxis);
    void setRotation(const Quaterion& quat);

    void scale(const Vec3f& delta);
    void setScale(const Vec3f& scale);

    Mat4 toMatrix() const;

    const Vec3f& forward() const;
    const Vec3f& up() const;
    const Vec3f& right() const;
private:
    void _updateBasis();

private:
    Vec3f _tranlsation;
    Quaterion _rotation;
    Vec3f _scale;

    Vec3f _forward;
    Vec3f _up;
    Vec3f _right;
};
}
