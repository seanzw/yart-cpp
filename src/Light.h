#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Ray.h"

class Light {
public:
    Light(vec3 color): c(color) {}
    virtual ~Light() {}

    virtual Ray genShadowRay(const vec3 &hit, float *t_out) = 0;
    virtual vec3 getColor(float t) = 0;

protected:
    vec3 c;
};

#endif