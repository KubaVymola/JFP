#ifndef __JCSSITLINTERFACE_H__
#define __JCSSITLINTERFACE_H__

#include <string>
#include "IFCS.h"

typedef void (*sayHello)();
typedef void (*init)();
typedef void (*loop)();
typedef void (*receiveData)(unsigned char *);
typedef void (*sendData)(unsigned char *);

class JCS_SITLInterface : public IFCS {
public:
    JCS_SITLInterface(const std::string& sharedObjectPath);
    ~JCS_SITLInterface();
    virtual void Iterate(FDMData &fdmData) const;
private:
    void * loadFunction(const std::string& symbolName);

    void * _libHandle;

    init _initFunc;
    loop _loopFunc;
    receiveData _receiveDataFunc;
    sendData _sendDataFunc;
    sayHello _sayHelloFunc;
};

#endif