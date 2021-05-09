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

#include <rendercomp/core/AbstractCamera.h>

namespace rendercomp
{
AbstractCamera::AbstractCamera(const float near, const float far, const float aspectRatio)
 : _view(glm::identity<Mat4>())
 , _projection(glm::identity<Mat4>())
 , _near(near)
 , _far(far)
 , _aspectRatio(aspectRatio)
{
    updateView();
}

void AbstractCamera::translate(const Vec3f &delta)
{
    _transform.translate(-delta);
}

void AbstractCamera::setTranslation(const Vec3f &translation)
{
    _transform.setTranslation(-translation);
}

Vec3f AbstractCamera::getPosition() const
{
    return Mat3(_view) * -Vec3f(_view[3]);
}

void AbstractCamera::moveForward(const float d)
{
    _transform.translate(_transform.forward() * -d);
}

void AbstractCamera::moveRight(const float d)
{
    _transform.translate(_transform.right() * -d);
}

void AbstractCamera::moveUp(const float d)
{
    _transform.translate(_transform.up() * -d);
}

void AbstractCamera::rotateX(const float degrees)
{
    _transform.rotateX(-degrees);
}

void AbstractCamera::rotateY(const float degrees)
{
    _transform.rotateY(-degrees);
}

void AbstractCamera::rotateZ(const float degrees)
{
    _transform.rotateZ(-degrees);
}

void AbstractCamera::rotate(const Quaterion &q)
{
    _transform.rotate(-q);
}

void AbstractCamera::setRotation(const Quaterion &q)
{
    _transform.setRotation(-q);
}

void AbstractCamera::setNearPlane(const float near)
{
    _near = near;
}

void AbstractCamera::setFarPlane(const float far)
{
    _far = far;
}

float AbstractCamera::getNearPlane() const
{
    return _near;
}

float AbstractCamera::getFarPlane() const
{
    return _far;
}

float AbstractCamera::getAspectRatio() const
{
    return _aspectRatio;
}

void AbstractCamera::setAspectRatio(const float ar)
{
    _aspectRatio = std::max<float>(ar, 0.01);
}

const Mat4& AbstractCamera::getProjectionMatrix() const
{
    return _projection;
}

const Mat4& AbstractCamera::getViewMatrix() const
{
    return _view;
}

void AbstractCamera::updateView()
{
    _view = _transform.toMatrix();
}

}
