#include "JSBSimInterface.h"

#include <sys/time.h>
#include <math.h>
#include "utils/Constants.h"
#include "FGJSBBase.h"
#include "models/FGAuxiliary.h"
#include "models/FGMassBalance.h"
#include "models/FGAccelerations.h"

#include "math/FGQuaternion.h"

// ######################### HELPER FUNCTIONS #########################

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

// ######################### END HELPER FUNCTIONS #########################


JSBSimInterface::JSBSimInterface(const std::string& inputFileName, FDMData& fdmData) {
    this->inputFileName = inputFileName;

    // ==== Implicit properties =====
    fdmData.RegisterFloat("time_s");
    fdmData.RegisterFloat("d_time_s");

    fdmData.RegisterDouble("longitude_deg");
    fdmData.RegisterDouble("latitude_deg");
    fdmData.RegisterDouble("altitude_asl_ft");

    fdmData.RegisterFloat("ecef_q_1");
    fdmData.RegisterFloat("ecef_q_2");
    fdmData.RegisterFloat("ecef_q_3");
    fdmData.RegisterFloat("ecef_q_4");

    fdmData.RegisterFloat("local_q_1");
    fdmData.RegisterFloat("local_q_2");
    fdmData.RegisterFloat("local_q_3");
    fdmData.RegisterFloat("local_q_4");

    fdmData.RegisterFloat("phi_deg");
    fdmData.RegisterFloat("theta_deg");
    fdmData.RegisterFloat("psi_deg");

    fdmData.RegisterFloat("alpha_deg");
    fdmData.RegisterFloat("beta_deg");

    fdmData.RegisterFloat("cg_x_m");
    fdmData.RegisterFloat("cg_y_m");
    fdmData.RegisterFloat("cg_z_m");

    // TODO replace with mapping
    fdmData.RegisterFloat("engine_pitch_rad");
    fdmData.RegisterFloat("engine_yaw_rad");
    // ==== END Implicit properties =====
}

JSBSimInterface::~JSBSimInterface() {

}


void JSBSimInterface::Init() {
    realtime = true;
    result = true;
    was_paused = false;
    play_nice = false;

    hasNewData = false;

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

    // Print all properties from JSBSim
    // string_list properties = FDMExec->GetPropertyCatalog();

    // for (auto property : properties) {
    //     std::cout << property << std::endl;
    // }
}

bool JSBSimInterface::CanIterate() {
    // return result && FDMExec->GetSimTime() <= end_time;
    return true;
}

bool JSBSimInterface::HasNewData() {
    // if (hasNewData) {
    //     hasNewData = false;
    //     return true;
    // }

    // return false;

    return hasNewData;
}

void JSBSimInterface::Iterate() {
    FDMExec->ProcessMessage();  // Process messages, if any.

    // Check if increment then hold is on and take appropriate actions if it is
    // Iterate is not supported in realtime - only in batch and playnice modes
    FDMExec->CheckIncrementalHold();

    // if running realtime, throttle the execution, else just run flat-out fast
    // unless "playing nice", in which case sleep for a while (0.01 seconds) each frame.
    // If suspended, then don't increment cumulative realtime "stopwatch".

    if (!FDMExec->Holding()) {
        if (!realtime) {  // BATCH MODE

            result = FDMExec->Run();

            if (play_nice)
                sim_nsleep(sleep_nseconds);
        } else {  // REALTIME MODE

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
                hasNewData = true;
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
        hasNewData = true;
        paused_seconds = getcurrentseconds() - current_seconds;
        sim_nsleep(sleep_nseconds);
        result = FDMExec->Run();
    }
}

void JSBSimInterface::UpdateData(FDMData& fdmData) {
    hasNewData = false;

    float newTime = FDMExec->GetSimTime();
    float deltaTime = newTime - fdmData.GetValue("time_s");

    // fdmData.time = FDMExec->GetSimTime();
    fdmData.SetValue("time_s", newTime);
    fdmData.SetValue("d_time_s", deltaTime);

    fdmData.SetValue("latitude_deg", FDMExec->GetPropagate()->GetLocation().GetLatitudeDeg());
    fdmData.SetValue("longitude_deg", FDMExec->GetPropagate()->GetLocation().GetLongitudeDeg());
    fdmData.SetValue("altitude_asl_ft", FDMExec->GetPropagate()->GetAltitudeASL());
    

    fdmData.SetValue("phi_deg", (float)(RAD_TO_DEG * FDMExec->GetPropagate()->GetEuler(JSBSim::FGJSBBase::ePhi)));
    fdmData.SetValue("theta_deg", (float)(RAD_TO_DEG * FDMExec->GetPropagate()->GetEuler(JSBSim::FGJSBBase::eTht)));
    fdmData.SetValue("psi_deg", (float)(RAD_TO_DEG * FDMExec->GetPropagate()->GetEuler(JSBSim::FGJSBBase::ePsi)));

    // fdmData.engine_pitch_rad = 0;

    // fdmData.SetValue("engine_pitch_rad", (float)(DEG_TO_RAD * (5 * sin(current_seconds))));
    // fdmData.SetValue("engine_yaw_rad", (float)(DEG_TO_RAD * (5 * cos(current_seconds))));


    FDMExec->GetPropertyManager()
         ->GetNode("propulsion/engine/pitch-angle-rad")->setDoubleValue(fdmData.GetValue("engine_pitch_rad"));
    FDMExec->GetPropertyManager()
        ->GetNode("propulsion/engine/yaw-angle-rad")->setDoubleValue(fdmData.GetValue("engine_yaw_rad"));

    // JSBSim::FGQuaternion attitudeQuaternion = FDMExec->GetPropagate()->GetQuaternion();
    JSBSim::FGQuaternion ECEFQuaternion = FDMExec->GetPropagate()->GetQuaternionECEF();
    fdmData.SetValue("ecef_q_1", ECEFQuaternion(1));
    fdmData.SetValue("ecef_q_2", ECEFQuaternion(2));
    fdmData.SetValue("ecef_q_3", ECEFQuaternion(3));
    fdmData.SetValue("ecef_q_4", ECEFQuaternion(4));

    JSBSim::FGQuaternion localQuaternion = FDMExec->GetPropagate()->GetQuaternion();
    fdmData.SetValue("local_q_1", localQuaternion(1));
    fdmData.SetValue("local_q_2", localQuaternion(2));
    fdmData.SetValue("local_q_3", localQuaternion(3));
    fdmData.SetValue("local_q_4", localQuaternion(4));

    fdmData.SetValue("alpha_deg", (float)(FDMExec->GetAuxiliary()->Getalpha(JSBSim::FGJSBBase::inDegrees)));
    fdmData.SetValue("beta_deg", (float)(FDMExec->GetAuxiliary()->Getbeta(JSBSim::FGJSBBase::inDegrees)));

    fdmData.SetFloat("cg_x_m", FDMExec->GetMassBalance()->GetXYZcg(1) * IN_TO_M);
    fdmData.SetFloat("cg_y_m", FDMExec->GetMassBalance()->GetXYZcg(2) * IN_TO_M);
    fdmData.SetFloat("cg_z_m", FDMExec->GetMassBalance()->GetXYZcg(3) * IN_TO_M);
}