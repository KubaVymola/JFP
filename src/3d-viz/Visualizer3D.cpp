#include "Visualizer3D.h"

#include <iostream>
#include "ShaderBank.h"
#include "utils/Constants.h"
#include "Camera.h"
#include "renderables/Line3D.h"
#include "renderables/Sphere.h"
#include "utils/Constants.h"

// Camera camera;

Camera Visualizer3D::camera;

bool Visualizer3D::_rightClicked = false;
bool Visualizer3D::_windowClosing = false;

int Visualizer3D::windowWidth;
int Visualizer3D::windowHeight;

Visualizer3D::Visualizer3D() : _hasClosed(false) {
    this->OpenWindow(1024.0f, 768.0f);

    _renderer = new Renderer();
    _renderer->Init();

    // ==== Cartesian-coordinates indicator (at origin) ====
    // X-coord
    _renderer->RegisterRenderable(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f)));
    // Y-coord                       
    _renderer->RegisterRenderable(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f)));

    // Z-coord
    _renderer->RegisterRenderable(new Line3D(glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f)));

    // ==== END Cartesian-coordinates indicator ====


    // ==== Cartesian-coordinates indicator (at origin) ====
    // X-coord
    _renderer->RegisterRenderable(new Line3D(glm::vec3(11.0f, 0.0f, 0.0f),
                                             glm::vec3(12.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f)));
    // Y-coord                       
    _renderer->RegisterRenderable(new Line3D(glm::vec3(11.0f, 0.0f, 0.0f),
                                             glm::vec3(11.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f)));

    // Z-coord
    _renderer->RegisterRenderable(new Line3D(glm::vec3(11.0f, 0.0f, 0.0f),
                                             glm::vec3(11.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f)));
    // ==== END Cartesian-coordinates indicator ====

    _renderer->RegisterRenderable(new Sphere(glm::vec3(0.0f, 0.0f, 0.0f), EARTH_RADIUS_M, 180, 360));
}

Visualizer3D::~Visualizer3D() {
}

void Visualizer3D::RegisterRenderable(IRenderable * renderable) {
    _renderer->RegisterRenderable(renderable);
}

void Visualizer3D::RegisterCameraProvider(ICameraProvider * cameraProvider) {
    _cameraProviders.push_back(cameraProvider);
}

void Visualizer3D::OpenWindow(float windowWidth, float windowHeight) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    std::cout << "Opening the 3D viz window" << std::endl;
    
    Visualizer3D::windowHeight = windowHeight;
    Visualizer3D::windowWidth = windowWidth;

    _window = glfwCreateWindow(windowWidth, windowHeight, "3D viz", NULL, NULL);

    std::cout << "Window created" << std::endl;

    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetFramebufferSizeCallback(_window, &Visualizer3D::framebuffer_size_callback);
    glfwSetCursorPosCallback(_window, &Visualizer3D::mousePositionCallback);
    glfwSetScrollCallback(_window, &Visualizer3D::mouseScrollCallback);
    glfwSetMouseButtonCallback(_window, &Visualizer3D::mouseButtonCallback);
    glfwSetWindowCloseCallback(_window, &Visualizer3D::windowCloseCallback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glEnable(GL_DEPTH_TEST);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    std::cout << "OpenGL version: " << major << "." << minor << std::endl;
    

    // glDepthRange(-1, 1);


    // glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    // glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    // glDepthFunc(GL_GEQUAL);
}

void Visualizer3D::Iterate() {
    if (Visualizer3D::_windowClosing) return;
    if (_hasClosed) return;

    float currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;
    
    glfwGetFramebufferSize(_window, &Visualizer3D::windowWidth, &Visualizer3D::windowHeight);
    glViewport(0, 0, Visualizer3D::windowWidth, Visualizer3D::windowHeight);
    
    this->updateCameraPos();
    processInput(_window);

    _renderer->Render(camera);

    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool Visualizer3D::ShouldClose() {
    return glfwWindowShouldClose(_window);
}

void Visualizer3D::Close() {
    if (!_hasClosed) {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwTerminate();
    }

    _hasClosed = true;
}

void Visualizer3D::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Visualizer3D::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
    camera.ProcessMouseMovement(xpos, ypos);
}

void Visualizer3D::mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void Visualizer3D::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(action == GLFW_PRESS && !Visualizer3D::_rightClicked) {
            Visualizer3D::_rightClicked = true;
            camera.ToggleFreeCamera(window);
        }
        if(action == GLFW_RELEASE) {
            Visualizer3D::_rightClicked = false;
        }
    } 

    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) {
            camera.SetLeftButtonHolding(true);
        }
        if(action == GLFW_RELEASE) {
            camera.SetLeftButtonHolding(false);
        }
    }
}

void Visualizer3D::windowCloseCallback(GLFWwindow* window) {
    Visualizer3D::_windowClosing = true;
}

void Visualizer3D::processInput(GLFWwindow *window) {
    // const float cameraSpeed = 2.5f * _deltaTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, _deltaTime);
        // _cameraPos += cameraSpeed * _cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, _deltaTime);
        // _cameraPos -= cameraSpeed * _cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::LEFT, _deltaTime);
        // _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, _deltaTime);
        // _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }
}

void Visualizer3D::updateCameraPos() {
    if (_currentCameraProviderId >= _cameraProviders.size()) {
        _currentCameraProviderId = 0;
    }

    if (_cameraProviders.size() == 0) return;
    
    Renderer::cameraPos
        = camera.GetPosition(_cameraProviders[_currentCameraProviderId]->GetCameraPosition());
}