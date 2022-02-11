#ifndef __JSBSIMINTERFACE_H__
#define __JSBSIMINTERFACE_H__

#include "IFDM.h"

class JSBSimInterface : IFDM {
public:
    void Init();
    void Iterate();
};


#endif