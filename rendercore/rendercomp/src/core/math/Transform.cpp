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
Transform::Transform()
 : _tranlsation(0.f, 0.f, 0.f)
 , _rotation(Quaterion())
 , _scale(1.f, 1.f, 1.f)
{
    _updateBasis();
}

void Transform::translate(const Vec3f& delta)
{
    _tranlsation += delta;
}

void Transform::setTranslation(const Vec3f& translation)
{
    _tranlsation = translation;
}

void Transform::rotateX(const float degree)
{
    _rotation *= Quaterion(Vec3f(glm::radians(degree), 0.f, 0.f));
    _updateBasis();
}

void Transform::rotateY(const float degree)
{
    _rotation *= Quaterion(Vec3f(0.f, glm::radians(degree), 0.f));
    _updateBasis();
}

void Transform::rotateZ(const float degree)
{
    _rotation *= Quaterion(Vec3f(0.f, 0.f, glm::radians(degree)));
    _updateBasis();
}

void Transform::rotate(const Vec3f& angleAxis)
{
    _rotation *= glm::toQuat(glm::eulerAngleXYX(glm::radians(angleAxis.x),
                                                glm::radians(angleAxis.y),
                                                glm::radians(angleAxis.z)));
    _updateBasis();
}

void Transform::rotate(const Quaterion& quat)
{
    _rotation *= quat;
    _updateBasis();
}

void Transform::setRotation(const Vec3f& angleAxis)
{
    _rotation = glm::toQuat(glm::eulerAngleXYX(glm::radians(angleAxis.x),
                                               glm::radians(angleAxis.y),
                                               glm::radians(angleAxis.z)));
    _updateBasis();
}

void Transform::setRotation(const Quaterion& quat)
{
    _rotation = quat;
    _updateBasis();
}

void Transform::scale(const Vec3f& delta)
{
    _scale *= delta;
    _scale = glm::max(_scale, Vec3f(0.01f, 0.01f, 0.01f));
}

void Transform::setScale(const Vec3f& scale)
{
    _scale = glm::max(scale, Vec3f(0.01f, 0.01f, 0.01f));
}

Mat4 Transform::toMatrix() const
{
    return glm::scale(_scale) * glm::translate(_tranlsation) * glm::toMat4(_rotation);
}

const Vec3f& Transform::forward() const
{
    return _forward;
}

const Vec3f& Transform::up() const
{
    return _up;
}

const Vec3f& Transform::right() const
{
    return _right;
}

void Transform::_updateBasis()
{
    const Mat3 rotMatrix = glm::toMat3(_rotation);
    _right = rotMatrix[0];
    _up = rotMatrix[1];
    _forward = rotMatrix[2];
}

}
