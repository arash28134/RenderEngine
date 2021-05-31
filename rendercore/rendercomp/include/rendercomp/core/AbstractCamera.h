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
     * @brief translate Moves the camera by the given translation vector. Because the
     *        camera view matrix must be inverted, this function performs internally the
     *        conversion to the opposite values, thus a normal displacement must be feed,
     *        rathern than the negative. (Example, to move 5 units on the axis Y, the
     *        parameter delta must be {0.f, 5.f, 0.f})
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param delta Vec3f containing the movement delta on each of the three world aligned axes
     */
    void translate(const Vec3f& delta) RC_NOEXCEPT;

    /**
     * @brief setTranslation Sets this camera position in the 3D space. Because the camera
     *        view matrix must be the inverted transform of what it must be achieved, this
     *        function performs internally the conversion to the opposite values, thus a normal
     *        3D position must be feed, rather than the negative. (Example, to place the camera
     *        in the position 10, 10, 10, the parameter translation must be {10.f, 10.f, 10.f})
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param translation Vec3f containing the new position of the camera
     */
    void setTranslation(const Vec3f& translation) RC_NOEXCEPT;

    /**
     * @brief moveForward Moves the camera 'd' units in the direction the camera is facing.
     *        Internally, the camera uses the forward vector of its transform to perform
     *        this operation. Negative values will move it backwards.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param d The amount of world space units to move the camera in the forward direction.
     */
    void moveForward(const float d) RC_NOEXCEPT;

    /**
     * @brief moveRight Moves the camera 'd' units to the right direction of the camera.
     *        Internally, the camera uses the right (strafe) vector of its transform to perform
     *        this operation. Negative values will move it to the left.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param d The amount of world space units to move the camera in the right direction.
     */
    void moveRight(const float d) RC_NOEXCEPT;

    /**
     * @brief moveUp Moves the camera 'd' units vertically.
     *        Internally, the camera uses the up vector of its transform to perform
     *        this operation. Negative values will move it down.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param d The amount of world space units to move the camera vertically.
     */
    void moveUp(const float d) RC_NOEXCEPT;

    /**
     * @brief rotateX Rotates the camera around its local X axis the given amount of degrees.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param degrees Amount of degrees to rotate.
     */
    void rotateX(const float degrees) RC_NOEXCEPT;

    /**
     * @brief rotateY Rotates the camera around its local Y axis the given amount of degrees.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param degrees Amount of degrees to rotate.
     */
    void rotateY(const float degrees) RC_NOEXCEPT;

    /**
     * @brief rotateZ Rotates the camera around its local Z axis the given amount of degrees.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param degrees Amount of degrees to rotate.
     */
    void rotateZ(const float degrees) RC_NOEXCEPT;

    /**
     * @brief rotate Performs the given rotation as a quaternion on top of the Camera's current
     *        local axis reference.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param q A quaternion representing a rotation to chain to the current camera's rotation.
     */
    void rotate(const Quaternion& q) RC_NOEXCEPT;

    /**
     * @brief setRotation Sets the camera orientation from the given quaternion parameter.
     *        Because the camera view matrix must be the opposite of the desired movement, this
     *        function internally negates the value to perform the correct movement without having
     *        to feed negative values to it.
     *        THIS FUNCTION DOES NOT UPDATE THE VIEW MATRIX. AFTER UPDATING THE VALUE, A MANUAL
     *        CALL TO updateView() MUST BE PERFORMED IF IT IS DESIRED TO UPDATE THE VIEW MATRIX.
     * @param q A quaternion representing the new camera rotation.
     */
    void setRotation(const Quaternion& q) RC_NOEXCEPT;

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
     * @brief getPosition Returns the wolrd space position of the camera
     * @return Vec3f containing the camera position
     */
    Vec3f getPosition() const RC_NOEXCEPT;


    /**
     * @brief Returns the vector pointing to the Z+ axis on the camera local reference
     *        system, transformed to world space
     * @return Vec3f with the normalized forward vector of the camera
     */
    const Vec3f& forward() const RC_NOEXCEPT;

    /**
     * @brief Returns the vector pointing to the X+ axis on the camera local reference
     *        system, transformed to world space
     * @return Vec3f with the normalized right vector of the camera
     */
    const Vec3f& right() const RC_NOEXCEPT;

    /**
     * @brief Returns the vector pointing to the Y+ axis on the camera local reference
     *        system, transformed to world space
     * @return Vec3f with the normalized up vector of the camera
     */
    const Vec3f& up() const RC_NOEXCEPT;

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

protected:
    Transform _transform;
    Vec3f _position;
    Mat4 _view;
    Mat4 _projection;
    float _near, _far;
    float _aspectRatio;

private:
    void _computePosition() RC_NOEXCEPT;

};
}
