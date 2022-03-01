#ifndef __FDMDATA_H__
#define __FDMDATA_H__

class FDMData {
public:
    double time;

    double longitude_deg;
    double latitude_deg;
    double altitude_asl_ft;

    float phi_deg;
    float theta_deg;
    float psi_deg;

    float alpha_deg;
    float beta_deg;

    // float phidot;
    // float thetadot;
    // float psidot;
    // float vcas;
    // float climb_rate;
    // float v_north;
    // float v_east;
    // float v_down;

    // float v_body_u;
    // float v_body_v;
    // float v_body_w;

    // float A_X_pilot;
    // float A_Y_pilot;
    // float A_Z_pilot;
};

#endif