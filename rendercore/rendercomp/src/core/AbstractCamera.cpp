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
AbstractCamera::AbstractCamera(const float near, const float far, const float aspectRatio) RC_NOEXCEPT
 : _view(glm::identity<Mat4>())
 , _projection(glm::identity<Mat4>())
 , _near(near)
 , _far(far)
 , _aspectRatio(aspectRatio)
{
    updateView();
}

void AbstractCamera::translate(const Vec3f &delta) RC_NOEXCEPT
{
    _transform.translate(-delta);
}

void AbstractCamera::setTranslation(const Vec3f &translation) RC_NOEXCEPT
{
    _transform.setTranslation(-translation);
}

void AbstractCamera::moveForward(const float d) RC_NOEXCEPT
{
    _transform.translate(_transform.forward() * -d);
}

void AbstractCamera::moveRight(const float d) RC_NOEXCEPT
{
    _transform.translate(_transform.right() * -d);
}

void AbstractCamera::moveUp(const float d) RC_NOEXCEPT
{
    _transform.translate(_transform.up() * -d);
}

void AbstractCamera::rotateX(const float degrees) RC_NOEXCEPT
{
    _transform.rotateX(-degrees);
}

void AbstractCamera::rotateY(const float degrees) RC_NOEXCEPT
{
    _transform.rotateY(-degrees);
}

void AbstractCamera::rotateZ(const float degrees) RC_NOEXCEPT
{
    _transform.rotateZ(-degrees);
}

void AbstractCamera::rotate(const Quaternion &q) RC_NOEXCEPT
{
    _transform.rotate(-q);
}

void AbstractCamera::setRotation(const Quaternion &q) RC_NOEXCEPT
{
    _transform.setRotation(-q);
}

void AbstractCamera::setNearPlane(const float near) RC_NOEXCEPT
{
    _near = near;
}

void AbstractCamera::setFarPlane(const float far) RC_NOEXCEPT
{
    _far = far;
}

float AbstractCamera::getNearPlane() const RC_NOEXCEPT
{
    return _near;
}

float AbstractCamera::getFarPlane() const RC_NOEXCEPT
{
    return _far;
}

float AbstractCamera::getAspectRatio() const RC_NOEXCEPT
{
    return _aspectRatio;
}

void AbstractCamera::setAspectRatio(const float ar) RC_NOEXCEPT
{
    _aspectRatio = std::max<float>(ar, 0.01);
}

const Mat4& AbstractCamera::getProjectionMatrix() const RC_NOEXCEPT
{
    return _projection;
}

const Mat4& AbstractCamera::getViewMatrix() const RC_NOEXCEPT
{
    return _view;
}

Vec3f AbstractCamera::getPosition() const RC_NOEXCEPT
{
    return Vec3f(glm::inverse(_view) * Vec4f(0.f, 0.f, 0.f, 1.f));
}

const Vec3f& AbstractCamera::forward() const RC_NOEXCEPT
{
    static Vec3f f = Vec3f(0.f, 0.f, -1.f);
    f = Vec3f(glm::inverse(_view) * Vec4f(0.f, 0.f, -1.f, 0.f));
    return f;
}

const Vec3f& AbstractCamera::right() const RC_NOEXCEPT
{
    return _transform.right();
}

const Vec3f& AbstractCamera::up() const RC_NOEXCEPT
{
    return _transform.up();
}

void AbstractCamera::updateView() RC_NOEXCEPT
{
    _view = _transform.toMatrix();
}

}
