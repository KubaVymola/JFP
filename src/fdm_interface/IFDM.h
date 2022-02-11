#ifndef __IFMD_H__
#define __IFMD_H__

class IFDM {
public:
    virtual void Init() = 0;
    virtual void Iterate() = 0;
};

#endif