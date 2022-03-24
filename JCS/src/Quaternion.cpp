#include "Quaternion.h"

#include "math.h"

Quaternion::Quaternion()
    : Quaternion(1.0f, 0.0f, 0.0f, 0.0f) {

}

Quaternion::Quaternion(Vec3 axis, float rotation) {

}

Quaternion::Quaternion(float yaw, float pitch, float roll) {
    double thtd2 = 0.5 * pitch; // theta
    double psid2 = 0.5 * yaw;   // psi
    double phid2 = 0.5 * roll;  // phi

    double Sthtd2 = sin(thtd2);
    double Spsid2 = sin(psid2);
    double Sphid2 = sin(phid2);

    double Cthtd2 = cos(thtd2);
    double Cpsid2 = cos(psid2);
    double Cphid2 = cos(phid2);

    double Cphid2Cthtd2 = Cphid2*Cthtd2;
    double Cphid2Sthtd2 = Cphid2*Sthtd2;
    double Sphid2Sthtd2 = Sphid2*Sthtd2;
    double Sphid2Cthtd2 = Sphid2*Cthtd2;

    data[0] = Cphid2Cthtd2*Cpsid2 + Sphid2Sthtd2*Spsid2;
    data[1] = Sphid2Cthtd2*Cpsid2 - Cphid2Sthtd2*Spsid2;
    data[2] = Cphid2Sthtd2*Cpsid2 + Sphid2Cthtd2*Spsid2;
    data[3] = Cphid2Cthtd2*Spsid2 - Sphid2Sthtd2*Cpsid2;

    Normalize();
}

Quaternion::Quaternion(float q1, float q2, float q3, float q4) {
    data[0] = q1;
    data[1] = q2;
    data[2] = q3;
    data[3] = q4;
}

Quaternion Quaternion::Inverse(void) const {
    float norm = GetMagnitude();

    return Quaternion(
        data[0],
        -data[1] / norm,
        -data[2] / norm,
        -data[3] / norm
    );
}

Quaternion Quaternion::Conjugate(void) const {
    return Quaternion(data[0], -data[1], -data[2], -data[3]);

    // return conjugatedQuaternion;
}

Quaternion Quaternion::NormalizeRet(void) const {
    float norm = GetMagnitude();

    if (norm == 0.0 || fabs(norm - 1.000) < 1e-10) return Quaternion(data[0], data[1], data[2], data[3]);

    return Quaternion(
        data[0] / norm,
        data[1] / norm,
        data[2] / norm, 
        data[3] / norm
    );
}

void Quaternion::Normalize(void) {
    float norm = GetMagnitude();

    if (norm == 0.0 || fabs(norm - 1.000) < 1e-10) return;

    data[0] /= norm;
    data[1] /= norm;
    data[2] /= norm; 
    data[3] /= norm;
}

float Quaternion::GetMagnitude(void) const {
    return sqrtf(data[0] * data[0]
               + data[1] * data[1]
               + data[2] * data[2]
               + data[3] * data[3]);
}

Vec3 Quaternion::ToEulerAngles(void) const {
    float yaw;
    float pitch;
    float roll;
    
    float w = data[0];
    float x = data[1];
    float y = data[2];
    float z = data[3];

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (w * y - z * x);
    if (abs(sinp) >= 1)
        pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    yaw = atan2(siny_cosp, cosy_cosp);

    return Vec3(yaw, pitch, roll);
}


Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(data[0] * q[0] - data[1] * q[1] - data[2] * q[2] - data[3] * q[3],
                      data[0] * q[1] + data[1] * q[0] + data[2] * q[3] - data[3] * q[2],
                      data[0] * q[2] - data[1] * q[3] + data[2] * q[0] + data[3] * q[1],
                      data[0] * q[3] + data[1] * q[2] - data[2] * q[1] + data[3] * q[0]);
}

const Quaternion& Quaternion::operator*=(const Quaternion& q) {
    double q0 = data[0] * q[0] - data[1] * q[1] - data[2] * q[2] - data[3] * q[3];
    double q1 = data[0] * q[1] + data[1] * q[0] + data[2] * q[3] - data[3] * q[2];
    double q2 = data[0] * q[2] - data[1] * q[3] + data[2] * q[0] + data[3] * q[1];
    double q3 = data[0] * q[3] + data[1] * q[2] - data[2] * q[1] + data[3] * q[0];

    data[0] = q0;
    data[1] = q1;
    data[2] = q2;
    data[3] = q3;
    
    return *this;
}

const Quaternion& Quaternion::operator/=(float scalar) {
    return operator*=(1.0/scalar);
}

const Quaternion& Quaternion::operator*=(float scalar) {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;

    return *this;
}

float Quaternion::operator()(int idx) const {
    return data[idx - 1];
}

float Quaternion::operator[](int idx) const {
    return data[idx];
}