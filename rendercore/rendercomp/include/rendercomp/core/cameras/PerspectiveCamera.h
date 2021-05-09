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
 * @brief The PerspectiveCamera class is the class that implements the perspective projection
 * functionality. It inherits from AbstractClass, which is in charge of the matrix view
 * handling.
 * This class stores and gives access to the projection parameters. Update of the projection
 * matrix using these parameters is done MANUALLY by the programmer, who must call the
 * updateProjection() method.
 */
class PerspectiveCamera : public AbstractCamera
{
public:
    /**
     * @brief PerspectiveCamera constructor. Initializes the view matrix to identity (via the
     *        superclass constructor), and initializes the projection matrix with the parameters
     *        given in this constructor.
     * @param near distance from the eye position to the near plane, in world units.
     * @param far distance from the eye position to the far plane, in world units
     * @param fovy vertical field of view angle, in degres. The horizontal field of view is
     *        computed automatically based on the vertical field of view and the aspect ratio.
     * @param aspectRatio Image aspect ratio that will be generated with this projection. It is
     *        computed as width / height of the image.
     */
    PerspectiveCamera(const float near, const float far, const float aspectRatio,
                      const float fovy);

    ~PerspectiveCamera() = default;

    PerspectiveCamera(const PerspectiveCamera&) = default;
    PerspectiveCamera& operator=(const PerspectiveCamera&) = default;

    PerspectiveCamera(PerspectiveCamera&&) = default;
    PerspectiveCamera& operator=(PerspectiveCamera&&) = default;

    /**
     * @brief setFovy sets the vertical field of view.
     *        THIS FUNCTION DOES NOT UPDATE THE PROJECTION MATRIX. UPDATE IS DONE BY MANUALLY
     *        CALLING updateProjection() method.
     * @param newFovy the value for the new field of view angle in degrees
     */
    void setFovy(const float newFovy);

    /**
     * @brief getFovy returns the current value of the vertical field of view (in degrees)
     * @return current vertical field of view (in degrees)
     */
    float getFovy() const;

    /**
     * @brief updateProjection Computes the projection matrix used the current projection values of
     *        this object. This function must be called MANUALLY everytime it is desired to update
     *        the projection matrix.
     */
    void updateProjection();

private:
    void _updateProjectionImpl();

    float _fovy;
};
}
