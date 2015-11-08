#ifndef POINT_LIGHT_HEADER
#define POINT_LIGHT_HEADER

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(vec3 color, vec3 position, vec3 attenuation) :
        Light(color), p(position), a(attenuation) {}

    virtual ~PointLight() {}

    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const;
    virtual tuple<Ray, vec3, float, float> genRay() const;
    virtual vec3 Le() const;

private:
    vec3 p, a;
};

#endif