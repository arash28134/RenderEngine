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

#include <rendercomp/core/math/Transform.h>

namespace rendercomp
{
Transform::Transform() RC_NOEXCEPT
 : _rotation(Quaternion())
 , _translation(0.f, 0.f, 0.f)
 , _scale(1.f, 1.f, 1.f)
{
    _updateBasis();
}

void Transform::translate(const Vec3f& delta) RC_NOEXCEPT
{
    _translation += delta;
    _updateBasis();
}

void Transform::setTranslation(const Vec3f& translation) RC_NOEXCEPT
{
    _translation = translation;
    _updateBasis();
}

void Transform::rotateX(const float degree) RC_NOEXCEPT
{
    _rotation *= Quaternion(Vec3f(glm::radians(degree), 0.f, 0.f));
    _updateBasis();
}

void Transform::rotateY(const float degree) RC_NOEXCEPT
{
    _rotation *= Quaternion(Vec3f(0.f, glm::radians(degree), 0.f));
    _updateBasis();
}

void Transform::rotateZ(const float degree) RC_NOEXCEPT
{
    _rotation *= Quaternion(Vec3f(0.f, 0.f, glm::radians(degree)));
    _updateBasis();
}

void Transform::rotate(const Vec3f& angleAxis) RC_NOEXCEPT
{
    _rotation *= glm::toQuat(glm::eulerAngleXYX(glm::radians(angleAxis.x),
                                                glm::radians(angleAxis.y),
                                                glm::radians(angleAxis.z)));
    _updateBasis();
}

void Transform::rotate(const Quaternion& quat) RC_NOEXCEPT
{
    _rotation *= quat;
    _updateBasis();
}

void Transform::setRotation(const Vec3f& angleAxis) RC_NOEXCEPT
{
    _rotation = glm::toQuat(glm::eulerAngleXYX(glm::radians(angleAxis.x),
                                               glm::radians(angleAxis.y),
                                               glm::radians(angleAxis.z)));
    _updateBasis();
}

void Transform::setRotation(const Quaternion& quat) RC_NOEXCEPT
{
    _rotation = quat;
    _updateBasis();
}

void Transform::scale(const Vec3f& delta) RC_NOEXCEPT
{
    _scale *= delta;
    _scale = glm::max(_scale, Vec3f(0.01f, 0.01f, 0.01f));
}

void Transform::setScale(const Vec3f& scale) RC_NOEXCEPT
{
    _scale = glm::max(scale, Vec3f(0.01f, 0.01f, 0.01f));
}

Mat4 Transform::toMatrix() const RC_NOEXCEPT
{
    return glm::scale(_scale) * glm::translate(_translation) * glm::toMat4(_rotation);
}

void Transform::_updateBasis() RC_NOEXCEPT
{
    const Mat3 rotMatrix = glm::toMat3(_rotation);
    /*
    _right = Vec3f(rotMatrix[0][0], rotMatrix[1][0], rotMatrix[2][0]);
    _up = Vec3f(rotMatrix[0][1], rotMatrix[1][1], rotMatrix[2][1]);
    _forward = Vec3f(rotMatrix[0][2], rotMatrix[1][2], rotMatrix[2][2]);
    */
    _right = rotMatrix[0];
    _up = rotMatrix[1];
    _forward = rotMatrix[2];
    _position = rotMatrix * _translation;
}

}
