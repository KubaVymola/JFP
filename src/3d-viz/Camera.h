#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float ZOOM        =  45.0f;
const float FREE_SENSITIVITY    = 0.08f;
const float ORBIT_SENSITIVITY   = 0.20f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float Distance;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0, 0.5, 1.0), glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), float yaw = YAW, float pitch = PITCH);

    // constructor with scalar values
    Camera(double posX, double posY, double posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    const glm::mat4 GetViewMatrix() const;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xPos, float yPos, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    void SetLeftButtonHolding(bool holding);

    glm::dvec3 GetPosition(glm::dvec3 orbitPoint);

    void ToggleFreeCamera(GLFWwindow *window);

private:
    // Default camera values
    glm::dvec3 Position;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    float _lastMouseX = 400;
    float _lastMouseY = 300;

    bool _leftButtonHolding = false;
    bool _forceUpdateLastXY = false;


    bool _freeCamera = false;
};
#endif