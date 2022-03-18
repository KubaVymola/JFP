#ifndef __FDMDATAMAPPING_H__
#define __FDMDATAMAPPING_H__

#include <string>

enum class FDMDataMappingType {
    integer_data,
    float_data
};

struct FDMDataMapping {
    const std::string FDMKey;
    const std::string JFPKey;
    const FDMDataMappingType type;
};

#endif