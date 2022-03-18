#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include "Camera.h"
#include "sim/FDMData.h"

class IRenderable {
public:
    virtual void Render(Camera * camera) const = 0;
};

#endif