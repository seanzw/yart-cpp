#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Ray.h"

class Light {
public:
    Light(vec3 color): c(color) {}
    virtual ~Light() {}

    virtual void genShadowRay(const vec3 &hit, vector<Ray> &rays) const = 0;
    virtual vec3 getColor(float t) const = 0;

protected:
    vec3 c;
};

#endif