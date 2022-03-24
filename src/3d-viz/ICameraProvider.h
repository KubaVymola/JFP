#ifndef __ICAMERAPROVIDER_H__
#define __ICAMERAPROVIDER_H__

#include "glm/vec3.hpp"

class ICameraProvider {
public:
    virtual glm::dvec3 GetCameraPosition() const = 0;
};

#endif