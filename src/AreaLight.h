/************************************************************************
 * Area Light for a circle.
 *
 ************************************************************************/

#ifndef AREA_LIGHT_HEADER
#define AREA_LIGHT_HEADER

#include "Light.h"

class AreaLight : public Light {
public:
    AreaLight(vec3 center, vec3 color, vec3 normal, float radius, int nSamples);
    virtual ~AreaLight() {}

    virtual void genShadowRay(const vec3 &hit, unique_ptr<vector<Ray> > &rays) const;
    virtual vec3 getColor(float t) const;

private:
    vec3 center;
    vec3 normal;
    float radius;
    int nSamples;

    // Local coordinate.
    vec3 xUnit;
    vec3 yUnit;
};

#endif
