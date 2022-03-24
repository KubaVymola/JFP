#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "Quaternion.h"
#include "Constants.h"

float enginePitch;
float engineYaw;

Quaternion attitude;
Quaternion targetAttitude;
float measurement_time;
float last_measurement_time;
float delta_t;

struct pid_controller_t {
    float k_p;
    float k_i;
    float k_d;

    float tau;

    float lim_max;
    float lim_min;

    float proportional;
    float differentiator;
    float integrator;
    float output;
    
    float prev_error;
    float prev_measurement;
};

void reset_pid(struct pid_controller_t &pid) {
    pid.differentiator = 0;
    pid.integrator = 0;
    pid.prev_error = 0;
    pid.prev_measurement = 0;
}

void init_pid(struct pid_controller_t &pid, float k_p, float k_i, float k_d, float tau, float lim_min, float lim_max) {
    pid.k_p = k_p;
    pid.k_i = k_i;
    pid.k_d = k_d;

    pid.tau = tau;
    pid.lim_min = lim_min;
    pid.lim_max = lim_max;

    reset_pid(pid);
}

float pid_update(struct pid_controller_t &pid, float set_point, float measurement, float delta_t) {
    float error = measurement - set_point;

    pid.proportional = pid.k_p * error;

    float limMaxInt = std::max(0.0f, pid.lim_max - pid.proportional);
    float limMinInt = std::min(0.0f, pid.lim_min - pid.proportional);
    
    pid.integrator = pid.integrator + 0.5 * pid.k_i * delta_t * (error + pid.prev_error);
    pid.integrator = std::min(limMaxInt, std::max(limMinInt, pid.integrator));

    pid.differentiator = (2 * pid.k_d * (measurement - pid.prev_measurement) 
        + (2 * pid.tau - delta_t) * pid.differentiator) / (2 * pid.tau + delta_t);

    pid.output = pid.proportional + pid.integrator + pid.differentiator;
    pid.output = std::max(pid.lim_min, std::min(pid.lim_max, pid.output));

    pid.prev_error = error;
    pid.prev_measurement = measurement;

    return pid.output;
}


struct pid_controller_t pitch_pid;
struct pid_controller_t yaw_pid;


extern "C" void init() {
    enginePitch = 0.0f;
    engineYaw = 0.0f;

    measurement_time = 0.0f;
    last_measurement_time = 0.0f;
    delta_t = 0.0f;

    targetAttitude = Quaternion(0.0f, 90.0f * DEG_TO_RAD, 0.0f);

    init_pid(pitch_pid, 0.3f, 0.0f, 0.25f, 0.15f, -10.0f, 10.0f);
    init_pid(yaw_pid,   0.3f, 0.0f, 0.25f, 0.15f, -10.0f, 10.0f);

    printf("JCS Initialized\n");
}


// Receive data from JCS
extern "C" void receiveData(unsigned char * binaryData) {
    float * data = (float *)binaryData;
    
    attitude = Quaternion(data[0], data[1], data[2], data[3]);

    measurement_time = data[4];

    // printf("JCS received pitch: %f, yaw: %f\n", pitch, yaw);
}

extern "C" void loop() {
    // Update times
    delta_t = measurement_time - last_measurement_time;
    last_measurement_time = measurement_time;
    

    // Quaternion difference = targetAttitude.Conjugate();
    Vec3 eulerAngles = (attitude * targetAttitude.Conjugate()).ToEulerAngles();

    printf("yw: %f, pt: %f, rl: %f\n",
           eulerAngles.GetYaw() * RAD_TO_DEG,
           eulerAngles.GetPitch() * RAD_TO_DEG,
           eulerAngles.GetRoll() * RAD_TO_DEG);

    // Update PIDs
    enginePitch = pid_update(pitch_pid, 0.0f, eulerAngles.GetPitch() * RAD_TO_DEG, delta_t);
    engineYaw   = pid_update(yaw_pid,   0.0f, eulerAngles.GetYaw() * RAD_TO_DEG, delta_t);
}

// Send data back to JCS
extern "C" void sendData(unsigned char * binaryData) {
    float data[7];
    
    data[5] = enginePitch;
    data[6] = engineYaw;

    memcpy(binaryData, data, sizeof(data));

    printf("JFP sending engine pitch: %f, yaw: %f\n", enginePitch, engineYaw);
}


extern "C" void sayHello() {
    printf("Hello from lib\n");
}

// Aruduino / ARM MCU template
int main() {
    init();

    while (1) {
        loop();
    }
}