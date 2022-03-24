#ifndef __VEC3_H__
#define __VEC3_H__

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3() : Vec3(0.0f, 0.0f, 0.0f) {
    }

    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float operator()(unsigned int idx) const {
        if (idx == 1) return x;
        if (idx == 2) return y;
        if (idx == 3) return z;

        return x;
    }

    float& operator()(unsigned int idx) {
        if (idx == 1) return x;
        if (idx == 2) return y;
        if (idx == 3) return z;

        return x;
    }

    float GetYaw() { return x; }
    float GetPitch() { return y; }
    float GetRoll() { return z; }
};

#endif