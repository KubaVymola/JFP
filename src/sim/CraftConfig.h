#ifndef __CRAFTCONFIG_H__
#define __CRAFTCONFIG_H__

#include <string>

struct CraftConfig {
    std::string FDMType;
    std::string FDMScriptPath;
    std::string FCSPath;
    int FDMSocketPort;

    std::string visualizationConfigPath;
};

#endif