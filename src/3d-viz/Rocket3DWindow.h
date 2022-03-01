#ifndef __ROCKET3DWINDOW_H__
#define __ROCKET3DWINDOW_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Rocket3DWindow {
public:
    Rocket3DWindow();
    ~Rocket3DWindow();
    void OpenWindow(float windowWidth, float windowHeight);
private: 
    float windowWidth;
    float windowHeight;
};

#endif