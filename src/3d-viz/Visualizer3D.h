#ifndef __VISUALIZER3D_H__
#define __VISUALIZER3D_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "ShaderBank.h"
#include "Renderer.h"
#include "IRenderable.h"
#include "ICameraProvider.h"

class Visualizer3D {
public:
    Visualizer3D();
    ~Visualizer3D();
    void OpenWindow(float windowWidth, float windowHeight);
    void Iterate();
    void RegisterCameraProvider(ICameraProvider * cameraProvider);
    void RegisterRenderable(IRenderable * renderable);
    bool ShouldClose();
    void Close();

    static int windowWidth;
    static int windowHeight;
private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
    static void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window);

    static bool _rightClicked;
    static bool _windowClosing;

    void processInput(GLFWwindow *window);
    void updateCameraPos();

    std::vector<ICameraProvider *> _cameraProviders;

    GLFWwindow * _window;
    ShaderBank * _shaderBank;
    Renderer * _renderer;
    
    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    int _currentCameraProviderId = 0;

    bool _hasClosed;
};

#endif