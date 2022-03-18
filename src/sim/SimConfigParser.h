#ifndef __SIMCONFIGPARSER_H__
#define __SIMCONFIGPARSER_H__

#include <string>
#include <vector>
#include "SimConfig.h"
#include "CraftConfig.h"

class SimConfigParser {
public:
    SimConfigParser();
    void ParseMainXML(const std::string& fileName, SimConfig& simConfig);
};

#endif