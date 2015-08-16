#ifndef RAY_HEADER
#define RAY_HEADER

#include "Const.h"

class Ray {
public:
    Ray(const vec3 &origin, const vec3 &direction) : d(direction), o(origin) {}
    ~Ray() {}

    const vec3 d;
    const vec3 o;
};

#endif