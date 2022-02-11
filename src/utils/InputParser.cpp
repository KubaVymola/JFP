#include "InputParser.h"

#include <algorithm>
#include <iostream>

InputParser::InputParser(int &argc, char ** argv) {
    for (int i = 1; i < argc; i++) {
        this->tokens.push_back(std::string(argv[i]));
        this->positionalArguments.push_back(i); 
    }
}

const std::string& InputParser::getCmdOption(const std::string &option) {
    auto itr = this->getIterator(option);

    itr = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != tokens.end() && ++itr != tokens.end()) {
        int toRemove = getToRemove(itr);

        this->positionalArguments.erase(
            std::remove(this->positionalArguments.begin(), this->positionalArguments.end(), toRemove),
            this->positionalArguments.end());
        this->positionalArguments.erase(
            std::remove(this->positionalArguments.begin(), this->positionalArguments.end(), toRemove - 1),
            this->positionalArguments.end());

        return *itr;
    }

    static const std::string emptyString("");
    return emptyString;
}

bool InputParser::cmdOptionExists(const std::string &option) {
    auto itr = this->getIterator(option);
    
    if (itr != this->tokens.end()) {
        int toRemove = getToRemove(itr);
        
        this->positionalArguments.erase(
            std::remove(this->positionalArguments.begin(), this->positionalArguments.end(), toRemove),
            this->positionalArguments.end());

        return true;
    }

    return false;
}

const std::vector<int>& InputParser::getRemainingPositionalArguments() const {
    return this->positionalArguments;
}


std::vector<std::string>::const_iterator InputParser::getIterator(const std::string &option) {
    return std::find(this->tokens.begin(), this->tokens.end(), option);
}

int InputParser::getToRemove(std::vector<std::string>::const_iterator itr) {
    return itr - tokens.begin() + 1;
}
