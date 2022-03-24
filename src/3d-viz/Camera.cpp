#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    Front = glm::vec3(-1.0f, 0.0f, 0.0f);
    WorldUp = up;
    Position = position;

    Yaw = yaw;
    Pitch = pitch;
    Zoom = ZOOM;
    Distance = 5.0f;

    MouseSensitivity = ORBIT_SENSITIVITY;
    MovementSpeed = SPEED;
    
    updateCameraVectors();
}

Camera::Camera(double posX, double posY, double posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    Front = glm::vec3(-1.0f, 0.0f, 0.0f);
    WorldUp = glm::vec3(upX, upY, upZ);
    Position = glm::vec3(posX, posY, posZ);

    Yaw = yaw;
    Pitch = pitch;
    Zoom = ZOOM;
    Distance = 5.0f;

    MovementSpeed = SPEED;
    MouseSensitivity = ORBIT_SENSITIVITY;

    updateCameraVectors();
}

const glm::mat4 Camera::GetViewMatrix() const {
    // return glm::lookAt(cameraPosition, cameraPosition + Front, Up);
    return glm::lookAt(glm::vec3(0.0f), Front, Up);
}


void Camera::ProcessMouseMovement(float xPos, float yPos, GLboolean constrainPitch) {
    if(!_freeCamera && !_leftButtonHolding) return;

    if(!_freeCamera && _forceUpdateLastXY) {
        _lastMouseX = xPos;
        _lastMouseY = yPos;
        
        _forceUpdateLastXY = false;
    }

    float xOffset = xPos - _lastMouseX;
    float yOffset = _lastMouseY - yPos;

    _lastMouseX = xPos;
    _lastMouseY = yPos;
    
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw   += xOffset;
    Pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    Distance -= (float)yoffset * 0.5f;

    // std::cout << Distance << std::endl;

    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;

    if (Distance > 15.0f)
        Distance = 15.0f;
    if (Distance < 1.0f) 
        Distance = 1.0f;
}


void Camera::SetLeftButtonHolding(bool holding) {
    _leftButtonHolding = holding;

    if(holding) {
        _forceUpdateLastXY = true;
    }
}

glm::dvec3 Camera::GetPosition(glm::dvec3 orbitPoint) {
    if(!_freeCamera) {
        Position = orbitPoint - (glm::dvec3)(Distance * Front);
    }

    return Position;
}

void Camera::ToggleFreeCamera(GLFWwindow *window) {
    _freeCamera = !_freeCamera;
    _forceUpdateLastXY = true;

    MouseSensitivity = _freeCamera
        ? FREE_SENSITIVITY
        : ORBIT_SENSITIVITY;

    if(_freeCamera) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(newFront);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}