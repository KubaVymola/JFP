#ifndef __MATRIX33_H__
#define __MATRIX33_H__

#include "Vec3.h"

class Matrix33 {
public:
    Matrix33(void);

    float operator()(unsigned int row, unsigned int col) const;
    float& operator()(unsigned int row, unsigned int col);

    Vec3 GetEuler(void) const;
private:
    float data[9];
};

#endif