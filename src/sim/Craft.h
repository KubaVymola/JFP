#ifndef __CRAFT_H__
#define __CRAFT_H__

#include "glm/vec3.hpp"
#include "3d-viz/CraftVisualizer.h"
#include "3d-viz/IRenderable.h"
#include "3d-viz/ICameraProvider.h"
#include "CraftConfig.h"
#include "IFDM.h"

class Craft : public IRenderable, public ICameraProvider {
public:
    Craft();
    void Init(const CraftConfig& craftConfig);
    bool CanIterate() const;
    void Iterate();
    virtual void Render(Camera * camera) const override;
    virtual glm::vec3 GetCameraPosition() const override;

    FDMData fdmData;
private:
    IFDM * fdmInterface;
    CraftVisualizer craftVisualizer;
};

#endif