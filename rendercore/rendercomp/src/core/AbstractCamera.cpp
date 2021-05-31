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

void AbstractCamera::updateView() RC_NOEXCEPT
{
    _view = glm::lookAt(_transform.worldPosition(),
                        _transform.worldPosition() + _transform.forward(),
                        _transform.up());
}

}
