#include "Matrix33.h"

#include <math.h>

Matrix33::Matrix33(void) {
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 0.0f;
    data[4] = 0.0f;
    data[5] = 0.0f;
    data[6] = 0.0f;
    data[7] = 0.0f;
    data[8] = 0.0f;
}


float Matrix33::operator()(unsigned int row, unsigned int col) const {
    return data[(col -1) * 3 + row - 1];
}

float& Matrix33::operator()(unsigned int row, unsigned int col) {
    return data[(col -1) * 3 + row - 1];
}

Vec3 Matrix33::GetEuler(void) const {
    Vec3 eulerAngles;
    bool GimbalLock = false;

    if (data[6] <= -1.0) {
        eulerAngles(2) = 0.5 * M_PI;
        GimbalLock = true;
    } else if (1.0 <= data[6]) {
        eulerAngles(2) = -0.5 * M_PI;
        GimbalLock = true;
    } else {
        eulerAngles(2) = asin(-data[6]);
    }

    if (GimbalLock) {
        eulerAngles(1) = atan2(-data[5], data[4]);
    } else {
        eulerAngles(1) = atan2(data[7], data[8]);
    }

    if (GimbalLock) {
        eulerAngles(3) = 0.0;
    } else {
        double psi = atan2(data[3], data[0]);

        if (psi < 0.0) {
            psi += 2 * M_PI;
        }

        eulerAngles(3) = psi;
    }

    return eulerAngles;
}