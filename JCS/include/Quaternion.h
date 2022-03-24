#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vec3.h"

class Quaternion {
public:
    Quaternion();
    Quaternion(Vec3 axis, float rotation);
    Quaternion(float yaw, float pitch, float roll);
    Quaternion(float q1, float q2, float q3, float q4);

    Quaternion Inverse(void) const;
    Quaternion Conjugate(void) const;
    Quaternion NormalizeRet(void) const;
    void Normalize(void);
    float GetMagnitude(void) const;

    Vec3 ToEulerAngles(void) const;

    Quaternion operator*(const Quaternion& q) const;
    const Quaternion& operator*=(const Quaternion& q);
    const Quaternion& operator/=(float scalar);
    const Quaternion& operator*=(float scalar);
    float operator()(int idx) const;
    float operator[](int idx) const;
    
private:
    float data[4];
};

#endif