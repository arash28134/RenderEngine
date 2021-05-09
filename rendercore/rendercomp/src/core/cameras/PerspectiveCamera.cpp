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

#include <rendercomp/core/cameras/PerspectiveCamera.h>

namespace rendercomp
{
PerspectiveCamera::PerspectiveCamera(const float near, const float far, const float aspectRatio,
                                     const float fovy)
 : AbstractCamera(near, far, aspectRatio)
 , _fovy(fovy)
{
    _updateProjectionImpl();
}

void PerspectiveCamera::setFovy(const float newFovy)
{
    _fovy = std::min<float>(std::max<float>(newFovy, 1.f), 180.f);
}

float PerspectiveCamera::getFovy() const
{
    return _fovy;
}

void PerspectiveCamera::updateProjection()
{
    _updateProjectionImpl();
}

void PerspectiveCamera::_updateProjectionImpl()
{
    const float radFovy = glm::radians(_fovy);
    _projection = Mat4(0.0f);
    _projection[0].x = 1.0f / tan(radFovy);
    _projection[1].y = 1.0f / tan(radFovy) * _aspectRatio;
    _projection[2].z = -(_far + _near) / (_far - _near);
    _projection[3].z = -2.f * _near * _far / (_far - _near);
    _projection[2].w = -1.0;
}
}
