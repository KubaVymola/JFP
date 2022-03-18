#ifndef __SIMCONFIG_H__
#define __SIMCONFIG_H__

#include <string>
#include <vector>
#include "CraftConfig.h"

struct SimConfig {
    std::string JFPMainXML;
    std::vector<CraftConfig> crafts;

};

#endif