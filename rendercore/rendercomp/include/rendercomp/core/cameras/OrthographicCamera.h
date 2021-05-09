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

#include "../AbstractCamera.h"

namespace rendercomp
{
/**
 * @brief The OrthographicCamera class is the class that implements the orthographic (parallel)
 * projection functionality. It inherits from AbstractClass, which is in charge of the matrix view
 * handling.
 * This class stores and gives access to the projection parameters. Update of the projection
 * matrix using these parameters is done MANUALLY by the programmer, who must call the
 * updateProjection() method.
 */
class OrthographicCamera : public AbstractCamera
{
public:
    /**
     * @brief OrthographicCamera Orthographic projection camera constructor. Initializes
     *        the view matrix to identity (done by the super class) and the projection
     *        matrix with the given parameters.
     * @param height Vertical length of the parallel frustum in world units that this camera must
     *        capture. The horizontal length will be computed using the aspectRatio parameter as
     *        width = height * aspectRatio
     * @param aspectRatio image aspect ratio (width / height)
     */
    OrthographicCamera(const float near, const float far,const float aspectRatio,
                       const float height);

    ~OrthographicCamera() = default;

    OrthographicCamera(const OrthographicCamera&) = default;
    OrthographicCamera& operator=(const OrthographicCamera&) = default;

    OrthographicCamera(OrthographicCamera&&) = default;
    OrthographicCamera& operator=(OrthographicCamera&&) = default;

    /**
     * @brief setHeight Sets the vertical length (in world units) of the parallel frustrum that this
     *        camera will capture.
     *        THIS FUNCTION DOES NOT UPDATE THE PROJECTION MATRIX. UPDATE IS DONE BY MANUALLY
     *        CALLING updateProjection() method.
     * @param height Vertical length in world units of the parallel frustrum of this camera
     */
    void setHeight(const float height);

    /**
     * @brief getHeight Returns the vertical length of this camera's parallel frustrum
     * @return the vertical frustrum length
     */
    float getHeight() const;

    /**
     * @brief updateProjection Computes the projection matrix used the current projection values of
     *        this object. This function must be called MANUALLY everytime it is desired to update
     *        the projection matrix.
     */
    void updateProjection();

private:
    void _updateProjectionImpl();

    float _height;
};
}
