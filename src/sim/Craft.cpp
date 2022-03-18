#include "Craft.h"

#include "JSBSimInterface.h"
#include "utils/Constants.h"

Craft::Craft() {

}

void Craft::Init(const CraftConfig& craftConfig) {
    if (craftConfig.FDMType == "JSBSim") {
        fdmInterface = new JSBSimInterface(craftConfig.FDMScriptFile, fdmData);
        fdmInterface->Init();
        std::cout << "Initialized JSBSim instance with input script " << craftConfig.FDMScriptFile << std::endl;

    }

    craftVisualizer.Init(craftConfig.visualizationMainFile);
    std::cout << "Initialized 3D visualizer from file " << craftConfig.visualizationMainFile << std::endl;    
}

bool Craft::CanIterate() const {
    return fdmInterface->CanIterate();
}

void Craft::Iterate() {
    fdmInterface->Iterate();

    if (fdmInterface->HasNewData()) {
        fdmInterface->UpdateData(fdmData);
    }
}

void Craft::Render(Camera * camera) const {
    craftVisualizer.Render(camera, fdmData);
}

glm::vec3 Craft::GetCameraPosition() const {
    return glm::vec3(
        fdmData.GetValue("latitude_deg") * EARTH_PERIPHERAL_CONSTANT,
        fdmData.GetValue("altitude_asl_ft") * FT_TO_M,
        fdmData.GetValue("longitude_deg") * EARTH_PERIPHERAL_CONSTANT
    );
}