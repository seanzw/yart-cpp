#ifndef RAY_HEADER
#define RAY_HEADER

#include "Const.h"

class Ray {
public:
    Ray(const vec3 &origin, const vec3 &direction, float min, float max)
        : d(direction), o(origin), tmin(min), tmax(max) {}
    ~Ray() {}

    const vec3 d;
    const vec3 o;
    const float tmin;
    const float tmax;
};

#endif