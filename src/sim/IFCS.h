#ifndef __IFCS_H__
#define __IFCS_H__

#include "FDMData.h"

class IFCS {
public:
    virtual void Iterate(FDMData &fdmData) const = 0;
};

#endif