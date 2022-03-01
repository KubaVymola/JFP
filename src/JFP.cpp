#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>

#include "initialization/FGTrim.h"
#include "network/SocketOutputFG.h"
#include "utils/InputParser.h"
#include "sim/FDMData.h"
#include "sim/IFDM.h"
#include "sim/JSBSimInterface.h"

// TODO make port CLI option
int PORT = 5550;
std::string jsbSimScriptFileName;
FDMData fdmData;
SocketOutputFG * socketOutputFG;

void parseCLIParams(int argc, char ** argv) {
    InputParser parser(argc, argv);

    // ? Key-value arguments
    const std::string cliPort = parser.getCmdOption("-p");
    if (!cliPort.empty()) PORT = atoi(cliPort.c_str());

    std::cout << PORT << std::endl;

    // ? Remaining positional arguments
    const std::vector<int> positionalArguments = parser.getRemainingPositionalArguments();

    int posArgId = 1;
    for (int i : positionalArguments) {
        switch (posArgId) {
            case 1:
                jsbSimScriptFileName = argv[i];
                break;
        }

        posArgId++;
    }
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
    std::cout << "Jakub's Flight Package, version 0.0.1" << std::endl; 
    
    parseCLIParams(argc, argv);

    if (jsbSimScriptFileName.empty()) return 1;
    socketOutputFG = new SocketOutputFG(PORT);
    
    std::cout << "Running JSBSim instance with input script " << jsbSimScriptFileName << std::endl;

    IFDM * fdmInterface = new JSBSimInterface(jsbSimScriptFileName);

    fdmInterface->Init();
    socketOutputFG->SendHeaders();
    socketOutputFG->SendData(fdmData);

    while (fdmInterface->CanIterate()) {
        fdmInterface->Iterate();

        if (fdmInterface->HasNewData()) {
            fdmInterface->UpdateData(fdmData);
            socketOutputFG->SendData(fdmData);
        }
    }

    return 0;
}