/************************************************************************
 * Area Light for a circle.
 *
 ************************************************************************/

#ifndef AREA_LIGHT_HEADER
#define AREA_LIGHT_HEADER

#include "Light.h"

class AreaLight : public Light {
public:
    AreaLight(vec3 center, vec3 color, vec3 normal, float radius);
    virtual ~AreaLight() {}

    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const;
    virtual tuple<Ray, vec3, float, float> genRay() const;
    virtual float pdf(const Intersection &hit, const vec3 &direction) const;
    virtual vec3 Le() const;

private:
    vec3 center;
    vec3 normal;
    float radius;
    float area;

    // Local coordinate.
    vec3 xUnit;
    vec3 yUnit;

    vec3 samplePoint() const;
};

#endif
