#ifndef __SOCKETOUTPUTFG_H__
#define __SOCKETOUTPUTFG_H__

#include "SocketOutput.h"
#include "../sim/FDMData.h"

class SocketOutputFG {
public:
    SocketOutputFG(const int port);
    ~SocketOutputFG();
    void SendHeaders() const;
    void SendData(FDMData& fdmData) const;
private:
    SocketOutput * socket;
};

#endif