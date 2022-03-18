#include "SimConfigParser.h"

#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

SimConfigParser::SimConfigParser() {

}

void SimConfigParser::ParseMainXML(const std::string& fileName, SimConfig& simConfig) {
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(fileName.c_str());

    XMLElement * root = xmlDoc.FirstChildElement("main");

    for (XMLElement * craft = root->FirstChildElement("craft");
         craft != NULL;
         craft = craft->NextSiblingElement("craft")) {

        CraftConfig newCraft;

        // Parse FDM
        XMLElement * fdmElem = craft->FirstChildElement("fdm");
        newCraft.FDMType = std::string(fdmElem->Attribute("type"));
        newCraft.FDMScriptFile = std::string(fdmElem->FirstChildElement("script")->GetText());

        // Parse Visualization
        XMLElement * visualizationElem = craft->FirstChildElement("visualization");
        newCraft.visualizationMainFile = std::string(visualizationElem->FirstChildElement("definition")->GetText());

        simConfig.crafts.push_back(newCraft);
    }
}