#ifndef __IFMD_H__
#define __IFMD_H__

#include "FDMData.h"

class IFDM {
public:
    virtual void Init() = 0;
    virtual bool CanIterate() = 0;
    virtual bool HasNewData() = 0;
    virtual void Iterate() = 0;
    virtual void UpdateData(FDMData& data) = 0;
};

#endif