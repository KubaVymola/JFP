#ifndef __CRAFTCONFIG_H__
#define __CRAFTCONFIG_H__

#include <string>

struct CraftConfig {
    std::string FDMType;
    std::string FDMScriptFile;
    int FDMSocketPort;

    std::string visualizationMainFile;
};

#endif