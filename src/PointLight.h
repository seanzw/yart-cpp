#ifndef POINT_LIGHT_HEADER
#define POINT_LIGHT_HEADER

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(vec3 color, vec3 position, vec3 attenuation) :
        Light(color), p(position), a(attenuation) {}

    virtual ~PointLight() {}

    virtual void genShadowRay(const vec3 &hit, vector<Ray> &rays) const;
    virtual vec3 getColor(float t) const;

private:
    vec3 p, a;
};

#endif