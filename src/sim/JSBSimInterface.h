#ifndef __JSBSIMINTERFACE_H__
#define __JSBSIMINTERFACE_H__

#include "IFDM.h"
#include "FGFDMExec.h"
#include <string>

class JSBSimInterface : IFDM {
public:
    JSBSimInterface(const std::string& inputFileName);
    ~JSBSimInterface();
    void Init();
    void Iterate();
private:
    JSBSim::FGFDMExec *FDMExec;

    std::string inputFileName;

    bool result;
    bool realtime;
    bool was_paused;
    bool play_nice;

    double frame_duration;

    double new_five_second_value;
    double actual_elapsed_time;
    double initial_seconds;
    double current_seconds;
    double paused_seconds;
    double sim_lag_time;
    double cycle_duration;
    double override_sim_rate_value;
    long sleep_nseconds;


    double end_time;
    double simulation_rate;
    bool override_sim_rate;
    double sleep_period;

    double getcurrentseconds(void);
    void sim_nsleep(long nanosec);
};


#endif