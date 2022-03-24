#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <thread>

#include "initialization/FGTrim.h"
#include "network/SocketOutputFG.h"
#include "utils/InputParser.h"
#include "sim/FDMData.h"
#include "sim/IFDM.h"
#include "sim/JSBSimInterface.h"
#include "sim/SimConfigParser.h"
#include "3d-viz/ICameraProvider.h"
#include "3d-viz/Visualizer3D.h"
#include "sim/Craft.h"
#include "sim/JCS_SITLInterface.h"

void parseCLIParams(int argc, char ** argv, SimConfig& simConfig) {
    InputParser parser(argc, argv);

    // ? Key-value arguments
    // const std::string cliPort = parser.getCmdOption("-p");
    // if (!cliPort.empty()) simConfig.FDMSocketPort = atoi(cliPort.c_str());

    // ? Remaining positional arguments
    const std::vector<int> positionalArguments = parser.getRemainingPositionalArguments();

    int posArgId = 1;
    for (int i : positionalArguments) {
        switch (posArgId) {
            case 1:
                simConfig.JFPMainXML = argv[i];
                break;
            case 2:
                if (simConfig.crafts.size() > 0) {
                    simConfig.crafts[0].FCSPath = argv[i];
                }
                break;
        }

        posArgId++;
    }
}

// void instantiateFDM(IFDM*& fdmInterface, SimConfig simConfig) {
//     if (simConfig.FDMType == "JSBSim") {
//         fdmInterface = new JSBSimInterface(simConfig.FDMInputFile);
//     }
// }

void JFPInit(const SimConfig& simConfig, std::vector<Craft *>& JFPcrafts, Visualizer3D& vizWindow) {
    for (auto craftConfig : simConfig.crafts) {
        // TODO informative exit exception (global)
        if (craftConfig.FDMScriptPath.empty()) continue;

        Craft * newCraft = new Craft();        
        newCraft->Init(craftConfig);

        vizWindow.RegisterRenderable(newCraft);
        vizWindow.RegisterCameraProvider((ICameraProvider *)newCraft);

        JFPcrafts.push_back(newCraft);
    }
}

void JFPLoop() {

}

/**
 * Functions:
 *  1. Parses CLI arguments
 *  2. Parses input XML file
 *  3. Instantiates objects
 *  4. Runs main loop
 * 
 * Usage: ./JFP <xml_input> [...override params]
 */
int main(int argc, char **argv) {
    SimConfig simConfig;
    std::vector<Craft *> crafts;

    std::cout << "Jakub's Flight Package, version 0.0.1" << std::endl; 

    // First run to get JFP main XML file
    parseCLIParams(argc, argv, simConfig);

    SimConfigParser configParser;
    configParser.ParseMainXML(simConfig.JFPMainXML, simConfig);

    // Second run to override config which is also provided via CLI
    parseCLIParams(argc, argv, simConfig);

    Visualizer3D vizWindow;

    JFPInit(simConfig, crafts, vizWindow);

    bool keepRunning = true;
    while (keepRunning) {
        keepRunning = false;
        
        // Update all crafts (at least one has to still be running)
        for (auto craft : crafts) {
            if (!craft->CanIterate()) continue;
            
            keepRunning = true;
            craft->Iterate();
        }

        if (!vizWindow.ShouldClose()) {
            vizWindow.Iterate();
        } else {
            vizWindow.Close();
        }
    }

    return 0;
}