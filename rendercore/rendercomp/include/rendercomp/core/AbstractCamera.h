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

#include "../common/Debug.h"

#include "math/Transform.h"

namespace rendercomp
{

/**
 * @brief The AbstractCamera class is the base class for any camera implementation.
 * It implements the common logic to compute and store the view matrix.
 * Although this class stores and give access to the projection matrix, is the subclasses
 * responsible to implement the projection logic computation, as well as to compute
 * such matrix in the way they see fitter.
 *
 * The camera looks towards the positive Z axis
 */
class AbstractCamera
{
public:
    /**
     * @brief AbstractCamera Common constructor for all camera subclasses.
     *        Initializes both the view and projection matrices to identity.
     */
    AbstractCamera(const float near, const float far, const float aspectRatio) RC_NOEXCEPT;

    virtual ~AbstractCamera() = default;

    AbstractCamera(const AbstractCamera&) RC_NOEXCEPT = default;
    AbstractCamera& operator=(const AbstractCamera&) RC_NOEXCEPT = default;

    AbstractCamera(AbstractCamera&&) RC_NOEXCEPT = default;
    AbstractCamera& operator=(AbstractCamera&&) RC_NOEXCEPT = default;

    /**
     * @brief setNearPlane sets the distance to the near plane, in world units.
     *        THIS FUNCTION DOES NOT UPDATE THE PROJECTION MATRIX. UPDATE IS DONE BY MANUALLY
     *        CALLING updateProjection() method.
     * @param near the distance from the eye position to the near plane in world units
     */
    void setNearPlane(const float near) RC_NOEXCEPT;

    /**
     * @brief setFarPlane sets the distance to the far plane, in world units.
     *        THIS FUNCTION DOES NOT UPDATE THE PROJECTION MATRIX. UPDATE IS DONE BY MANUALLY
     *        CALLING updateProjection() method.
     * @param far the distance from the eye position to the near plane in world units
     */
    void setFarPlane(const float far) RC_NOEXCEPT;

    /**
     * @brief setAspectRatio sets the aspect ratio (output width / output height) that
     *        this projection will be used to map to.
     *        THIS FUNCTION DOES NOT UPDATE THE PROJECTION MATRIX. UPDATE IS DONE BY MANUALLY
     *        CALLING updateProjection() method.
     * @param ar aspect ratio of the image (width / height)
     */
    void setAspectRatio(const float ar) RC_NOEXCEPT;

    /**
     * @brief getNearPlane returns the current distance from the eye to the near projection plane
     * @return distance from eye to near projection plane
     */
    float getNearPlane() const RC_NOEXCEPT;

    /**
     * @brief getFarPlane returns the current distance from the eye to the far projection plane
     * @return distance from eye to far projection plane
     */
    float getFarPlane() const RC_NOEXCEPT;

    /**
     * @brief getAspectRatio returns the current setted image aspect ratio used for this projection
     * @return current used aspect ratio to compute the projection matrix
     */
    float getAspectRatio() const RC_NOEXCEPT;

    /**
     * @brief getProjectionMatrix Returns a reference to the last computed projection matrix.
     *        This class does not update the projection matrix, that is the responsability
     *        of the subclasses, which implement the specific logic for the projection.
     * @return a const Mat4 object with the projection matrix.
     */
    const Mat4& getProjectionMatrix() const RC_NOEXCEPT;

    /**
     * @brief getViewMatrix Returns a reference to the last computed view matrix.
     *        The view matrix is NOT updated automaticlly. It must be done manually
     *        by calling updateView() method.
     * @return a const Mat4 object with the view matrix.
     */
    const Mat4& getViewMatrix() const RC_NOEXCEPT;

    /**
     * @brief updateView Computes and stores the view matrix by composing it from this camera's
     *        rotation and translation.
     */
    void updateView() RC_NOEXCEPT;

    /**
     * @brief updateProjection Computes and stores the projection matrix by composing it from
     *        the camera's projection parameter
     */
    virtual void updateProjection() RC_NOEXCEPT = 0;

    /**
     * @brief Returns a reference to the transform object to manipulate the camera
     * @return Transform object
     */
    const Transform& transform() const RC_NOEXCEPT { return _transform; }

    /**
     * @brief Returns a reference to the transform object to manipulate the camera
     * @return Transform object
     */
    Transform& transform() RC_NOEXCEPT { return _transform; }

protected:
    Transform _transform;
    Mat4 _view;
    Mat4 _projection;
    float _near, _far;
    float _aspectRatio;

private:
    void _computePosition() RC_NOEXCEPT;

};
}
