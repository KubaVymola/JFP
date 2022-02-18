#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <iostream>

#include "FGFDMExec.h"
#include "initialization/FGTrim.h"
#include "network/SocketOutput.h"
#include "utils/InputParser.h"

// TODO make port CLI option
const int PORT = 5550;

void parseCLIParams(int argc, char ** argv) {
    InputParser parser(argc, argv);

    const std::string myOption = parser.getCmdOption("-f");
    parser.cmdOptionExists("-h");
    const std::vector<int> positionalArguments = parser.getRemainingPositionalArguments();

    std::cout << "Positional args:" << std::endl;
    for (int i : positionalArguments) {
        std::cout << i << ": " << argv[i] << std::endl;
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


    SocketOutput socket_output;
    socket_output.create_socket(PORT);
    socket_output.send_headers();
    std::cout << "Done!" << std::endl;

    return 0;
}