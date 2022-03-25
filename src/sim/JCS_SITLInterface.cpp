#include "JCS_SITLInterface.h"

#include <iostream>
#include <dlfcn.h>
#include "utils/Constants.h"
#include "math/FGMatrix33.h"
#include "math/FGQuaternion.h"


JCS_SITLInterface::JCS_SITLInterface(const std::string& sharedObjectPath) {
    _libHandle = dlopen(sharedObjectPath.c_str(), RTLD_NOW);
    if (_libHandle == nullptr) std::cout << "Bad handle" << std::endl;

    
    _sayHelloFunc = reinterpret_cast<sayHello>(loadFunction("sayHello"));
    _initFunc = reinterpret_cast<init>(loadFunction("init"));
    _loopFunc = reinterpret_cast<loop>(loadFunction("loop"));
    _sendDataFunc = reinterpret_cast<sendData>(loadFunction("sendData"));
    _receiveDataFunc = reinterpret_cast<receiveData>(loadFunction("receiveData"));

    _sayHelloFunc();
    _initFunc();
}

JCS_SITLInterface::~JCS_SITLInterface() {
    dlclose(_libHandle);
}

void JCS_SITLInterface::Iterate(FDMData &fdmData) const {
    float data[7];
    // Buffer to send and receive data
    unsigned char binaryData[sizeof(data) / (sizeof(unsigned char))];
    

    // JSBSim::FGQuaternion attitudeQuat(fdmData.GetValue("phi_deg") * DEG_TO_RAD,
    //                                   fdmData.GetValue("theta_deg") * DEG_TO_RAD,
    //                                   fdmData.GetValue("psi_deg") * DEG_TO_RAD);

    JSBSim::FGQuaternion attitudeQuat;
    attitudeQuat(1) = fdmData.GetValue("local_q_1");
    attitudeQuat(2) = fdmData.GetValue("local_q_2");
    attitudeQuat(3) = fdmData.GetValue("local_q_3");
    attitudeQuat(4) = fdmData.GetValue("local_q_4");

    // attitudeQuat *= JSBSim::FGQuaternion(2, -90.0 * DEG_TO_RAD);
    
    float phi_got = attitudeQuat.GetEuler(1) * RAD_TO_DEG;
    float theta_got = attitudeQuat.GetEuler(2) * RAD_TO_DEG;
    float psi_got = attitudeQuat.GetEuler(3) * RAD_TO_DEG;

    // std::cout << "Got yaw " << phi_got
    //           << ", Got pitch " << theta_got
    //           << ", Got roll " << psi_got
    //           << std::endl;

    data[0] = fdmData.GetValue("local_q_1");
    data[1] = fdmData.GetValue("local_q_2");
    data[2] = fdmData.GetValue("local_q_3");
    data[3] = fdmData.GetValue("local_q_4");
    data[4] = fdmData.GetValue("time_s");

    // Send data to FCS
    memcpy(binaryData, data, sizeof(data));
    _receiveDataFunc(binaryData);

    // Run FCS SITL iteration
    _loopFunc();

    // Receive data from FCS
    _sendDataFunc(binaryData);
    memcpy(data, binaryData, sizeof(data));

    // Update FDM state (will get moved elsewhere)
    fdmData.SetValue("engine_pitch_rad", data[5] * DEG_TO_RAD);
    fdmData.SetValue("engine_yaw_rad", data[6] * DEG_TO_RAD);
}

void * JCS_SITLInterface::loadFunction(const std::string& symbolName) {
    void * func = dlsym(_libHandle, symbolName.c_str());
     
    if (func == nullptr) std::cout << "Bad func handle" << std::endl;
    
    return reinterpret_cast<void *>(func);
}