#ifndef __INPUTPARSER_H__
#define __INPUTPARSER_H__

#include <string>
#include <vector>

/**
 * Source: https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
 */

class InputParser {
public:
    InputParser(int &argc, char ** argv);
    const std::string& getCmdOption(const std::string &option);
    bool cmdOptionExists(const std::string &option);
    const std::vector<int>& getRemainingPositionalArguments() const;
private:
    std::vector<std::string>::const_iterator getIterator(const std::string &option);
    int getToRemove(std::vector<std::string>::const_iterator itr);

    std::vector<std::string> tokens;
    std::vector<int> positionalArguments;
};

#endif