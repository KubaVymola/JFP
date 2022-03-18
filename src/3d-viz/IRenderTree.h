#ifndef __IRENDERTREE_H__
#define __IRENDERTREE_H__

#include "glm/mat4x4.hpp"
#include "Camera.h"
#include "sim/FDMData.h"

class IRenderTree {
public:
    virtual void AddChild(IRenderTree * child) = 0;
    virtual void Render(Camera * camera,
                        FDMData fdmData,
                        glm::mat4 rootTransform,
                        glm::mat4 parentTransform) const = 0;
};

#endif