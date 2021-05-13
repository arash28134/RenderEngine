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

#include <rendercomp/core/cameras/OrthographicCamera.h>

namespace rendercomp
{
OrthographicCamera::OrthographicCamera(const float near, const float far, const float aspectRatio,
                                       const float height)
 : AbstractCamera(near, far, aspectRatio)
 , _height(height)
{
    _updateProjectionImpl();
}

void OrthographicCamera::setHeight(const float height)
{
    _height = std::max<float>(height, 0.01f);
}

void OrthographicCamera::updateProjection()
{
    _updateProjectionImpl();
}

void OrthographicCamera::_updateProjectionImpl()
{
    const float width = _height * _aspectRatio;

    _projection = Mat4(0.0f);
    _projection[0].x = 2.f / width;
    _projection[1].y = 2.f / _height;
    _projection[2].z = -2.f / (_far - _near);
    _projection[3].z = -(_near + _far) / (_far - _near);
    _projection[3].w = 1.f;
}
}
