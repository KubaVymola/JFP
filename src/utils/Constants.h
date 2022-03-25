#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <math.h>

const double RAD_TO_DEG = 180. / M_PI;
const double DEG_TO_RAD = M_PI / 180.;

const double EARTH_RADIUS_M = 6'378'000;
const double EARTH_PERIPHERAL_CONSTANT = EARTH_RADIUS_M * M_PI / 180;

const double FT_TO_M = 0.3048;
const double IN_TO_M = 0.0254;
const double SLUG_TO_KG = 14.5939029;

#endif