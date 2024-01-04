#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;

        //cele 3 axe ale sistemului de coordonate camera
        this->cameraUpDirection = glm::normalize(cameraUp);
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    glm::vec3 Camera::getCameraPosition() {
        return this->cameraPosition;
    }


    void Camera::updateDirection(glm::vec3 cameraTarget) {
        this->cameraTarget = cameraTarget;
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));

    }


    //update the camera internal paarameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD: this->cameraPosition.z -= speed; this->cameraTarget.z -= speed; break;
        case MOVE_BACKWARD: this->cameraPosition.z += speed; this->cameraTarget.z += speed; break;
        case MOVE_LEFT: this->cameraPosition.x += speed; this->cameraTarget.x += speed; break;
        case MOVE_RIGHT: this->cameraPosition.x -= speed; this->cameraTarget.x -= speed; break;
        default: this->cameraPosition = this->cameraPosition;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        glm::mat4 x_rotation = glm::mat4(1.0f);
        glm::mat4 y_rotation = glm::mat4(1.0f);

        x_rotation = glm::rotate(x_rotation, yaw, glm::vec3(1.0f, 0.0f, 0.0f));
        y_rotation = glm::rotate(y_rotation, pitch, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec4 pos = x_rotation * y_rotation * glm::vec4(this->cameraPosition, 1.0f);
        glm::vec4 target = x_rotation * y_rotation * glm::vec4(this->cameraTarget, 1.0f);

        this->cameraPosition = glm::vec3(pos.x, pos.y, pos.z);
        this->cameraTarget = glm::vec3(pos.x, pos.y, pos.z);

        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));
        this->cameraUpDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraRightDirection));
    }
}