#ifndef POINT_LIGHT_HEADER
#define POINT_LIGHT_HEADER

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(vec3 color, vec3 position, vec3 attenuation) :
        Light(color), p(position), a(attenuation) {}

    virtual ~PointLight() {}

    virtual void genShadowRay(const Intersection &hit, vector<pair<Ray, float> > &rayPDFs) const;
    virtual vec3 Le(float t) const;

private:
    vec3 p, a;
};

#endif