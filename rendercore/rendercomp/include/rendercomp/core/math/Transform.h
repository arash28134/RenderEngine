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

#include "../../common/Debug.h"
#include "../../common/Types.h"

namespace rendercomp
{
class Transform
{
public:
    Transform() RC_NOEXCEPT;

    Transform(const Transform&) RC_NOEXCEPT = default;
    Transform(Transform&&) RC_NOEXCEPT = default;

    Transform& operator=(const Transform&) RC_NOEXCEPT = default;
    Transform& operator=(Transform&&) RC_NOEXCEPT = default;

    ~Transform() = default;

    void translate(const Vec3f& delta) RC_NOEXCEPT;
    void setTranslation(const Vec3f& translation) RC_NOEXCEPT;

    void rotateX(const float degree) RC_NOEXCEPT;
    void rotateY(const float degree) RC_NOEXCEPT;
    void rotateZ(const float degree) RC_NOEXCEPT;
    void rotate(const Vec3f& angleAxis) RC_NOEXCEPT;
    void rotate(const Quaternion& quat) RC_NOEXCEPT;
    void setRotation(const Vec3f& angleAxis) RC_NOEXCEPT;
    void setRotation(const Quaternion& quat) RC_NOEXCEPT;

    void scale(const Vec3f& delta) RC_NOEXCEPT;
    void setScale(const Vec3f& scale) RC_NOEXCEPT;

    Mat4 toMatrix() const RC_NOEXCEPT;

    /**
     * @brief Returns the cartesian coordinates, in world space, represented by
     *        this transform
     */
    const Vec3f& worldPosition() const RC_NOEXCEPT { return _position; }

    /**
     * @brief Returns the forward orientation, in world space, respresented by this transform
     */
    const Vec3f& forward() const RC_NOEXCEPT { return _forward; }

    /**
     * @brief Returns the upwards orientation, in world space, represented by this transform
     */
    const Vec3f& up() const RC_NOEXCEPT { return _up; }

    /**
     * @brief Returns the rightwards orientation, in world space, represented by this transform
     */
    const Vec3f& right() const RC_NOEXCEPT { return _right; };

    /**
     * @brief Returns the translation vector of this transform
     */
    const Vec3f& translation() const RC_NOEXCEPT { return _translation; }

    /**
     * @brief Returns the quaternion that represents the rotation of this transform
     */
    const Quaternion& rotation() const RC_NOEXCEPT { return _rotation; }

    /**
     * @brief Returns the scale vector that represents the scale of this transform
     */
    const Vec3f& scale() const RC_NOEXCEPT { return _scale; }
private:
    void _updateBasis() RC_NOEXCEPT;

private:
    Quaternion _rotation;
    Vec3f _translation;
    Vec3f _scale;

    Vec3f _position;
    Vec3f _forward;
    Vec3f _up;
    Vec3f _right;
};
}
