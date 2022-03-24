#include "Craft.h"

#include "JSBSimInterface.h"
#include "utils/Constants.h"
#include "JCS_SITLInterface.h"

Craft::Craft() {

}

void Craft::Init(const CraftConfig& craftConfig) {
    if (craftConfig.FDMType == "JSBSim") {
        fdmInterface = new JSBSimInterface(craftConfig.FDMScriptPath, fdmData);
        fdmInterface->Init();
        std::cout << "Initialized JSBSim instance with input script " << craftConfig.FDMScriptPath << std::endl;

    }

    _fcsInterface = new JCS_SITLInterface(craftConfig.FCSPath);

    craftVisualizer.Init(craftConfig.visualizationConfigPath);
    std::cout << "Initialized 3D visualizer from file " << craftConfig.visualizationConfigPath << std::endl;    
}

bool Craft::CanIterate() const {
    return fdmInterface->CanIterate();
}

void Craft::Iterate() {
    fdmInterface->Iterate();

    if (fdmInterface->HasNewData()) {
        fdmInterface->UpdateData(fdmData);
        _fcsInterface->Iterate(fdmData);
    }
}

void Craft::Render(Camera * camera) const {
    craftVisualizer.Render(camera, fdmData);
}

glm::dvec3 Craft::GetCameraPosition() const {
    return glm::dvec3(
        fdmData.GetValue("latitude_deg") * EARTH_PERIPHERAL_CONSTANT,
        fdmData.GetValue("altitude_asl_ft") * FT_TO_M,
        fdmData.GetValue("longitude_deg") * EARTH_PERIPHERAL_CONSTANT
    );
}