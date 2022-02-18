#include "JSBSimInterface.h"

#include <sys/time.h>

JSBSimInterface::JSBSimInterface(const std::string& inputFileName) {
    this->inputFileName = inputFileName;
}

JSBSimInterface::~JSBSimInterface() {

}


void JSBSimInterface::Init() {
    realtime = true;
    result = true;
    was_paused = false;
    play_nice = false;

    new_five_second_value = 0.0;
    actual_elapsed_time = 0;
    initial_seconds = 0;
    current_seconds = 0.0;
    paused_seconds = 0.0;
    sim_lag_time = 0;
    cycle_duration = 0.0;
    override_sim_rate_value = 0.0;
    sleep_nseconds = 0;

    end_time = 1e99;
    simulation_rate = 1. / 120.;
    override_sim_rate = false;
    sleep_period = 0.01;
    

    FDMExec = new JSBSim::FGFDMExec();

    if (simulation_rate < 1.0)
        FDMExec->Setdt(simulation_rate);
    else
        FDMExec->Setdt(1.0 / simulation_rate);

    // TODO: Allow traditional JSBSim output
    // Allow user to specify output definition file
    // if (argc > 2) {
    //     FDMExec->SetOutputDirectives(SGPath(argv[2]));
    // }

    FDMExec->LoadScript(SGPath(inputFileName.c_str()));
    FDMExec->RunIC();
    FDMExec->PrintSimulationConfiguration();
    FDMExec->GetPropagate()->DumpState();

    result = FDMExec->Run();  // MAKE AN INITIAL RUN

    frame_duration = FDMExec->GetDeltaT();
    if (realtime)
        sleep_nseconds = (long)(frame_duration * 1e9);
    else
        sleep_nseconds = (sleep_period)*1e9;  // 0.01 seconds

    tzset();
    current_seconds = initial_seconds = getcurrentseconds();

}

void JSBSimInterface::Iterate() {
    while (result && FDMExec->GetSimTime() <= end_time) {
        FDMExec->ProcessMessage();  // Process messages, if any.

        // Check if increment then hold is on and take appropriate actions if it is
        // Iterate is not supported in realtime - only in batch and playnice modes
        FDMExec->CheckIncrementalHold();

        // if running realtime, throttle the execution, else just run flat-out fast
        // unless "playing nice", in which case sleep for a while (0.01 seconds) each frame.
        // If suspended, then don't increment cumulative realtime "stopwatch".

        if (!FDMExec->Holding()) {
            if (!realtime) {  // ------------ RUNNING IN BATCH MODE

                result = FDMExec->Run();

                if (play_nice)
                    sim_nsleep(sleep_nseconds);
            } else {  // ------------ RUNNING IN REALTIME MODE

                // "was_paused" will be true if entering this "run" loop from a paused state.
                if (was_paused) {
                    initial_seconds += paused_seconds;
                    was_paused = false;
                }
                current_seconds = getcurrentseconds();                            // Seconds since 1 Jan 1970
                actual_elapsed_time = current_seconds - initial_seconds;          // Real world elapsed seconds since start
                sim_lag_time = actual_elapsed_time - FDMExec->GetSimTime();       // How far behind sim-time is from actual
                                                                                  // elapsed time.
                for (int i = 0; i < (int)(sim_lag_time / frame_duration); i++) {  // catch up sim time to actual elapsed time.
                    result = FDMExec->Run();
                    cycle_duration = getcurrentseconds() - current_seconds;  // Calculate cycle duration
                    current_seconds = getcurrentseconds();                   // Get new current_seconds
                    if (FDMExec->Holding())
                        break;
                }

                if (play_nice)
                    sim_nsleep(sleep_nseconds);

                if (FDMExec->GetSimTime() >= new_five_second_value) {  // Print out elapsed time every five seconds.
                    cout << "Simulation elapsed time: " << FDMExec->GetSimTime() << endl;
                    new_five_second_value += 5.0;
                }
            }
        } else {  // Suspended
            was_paused = true;
            paused_seconds = getcurrentseconds() - current_seconds;
            sim_nsleep(sleep_nseconds);
            result = FDMExec->Run();
        }
    }
}


#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
double JSBSimInterface::getcurrentseconds(void) {
    struct timeb tm_ptr;
    ftime(&tm_ptr);
    return tm_ptr.time + tm_ptr.millitm * 0.001;
}
#else
double JSBSimInterface::getcurrentseconds(void) {
    struct timeval tval;
    struct timezone tz;

    gettimeofday(&tval, &tz);
    return (tval.tv_sec + tval.tv_usec * 1e-6);
}
#endif

#if defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW32__)
void JSBSimInterface::sim_nsleep(long nanosec) {
    Sleep((DWORD)(nanosec * 1e-6));  // convert nanoseconds (passed in) to milliseconds for Win32.
}
#else
void JSBSimInterface::sim_nsleep(long nanosec) {
    struct timespec ts, ts1;

    ts.tv_sec = 0;
    ts.tv_nsec = nanosec;
    nanosleep(&ts, &ts1);
}
#endif