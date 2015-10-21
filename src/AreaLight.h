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

    virtual void genShadowRay(const Intersection &hit, vector<pair<Ray, float> > &rayPDFs) const;
    virtual vec3 Le(float t) const;

private:
    vec3 center;
    vec3 normal;
    float radius;
    int nSamples;
    float area;

    // Local coordinate.
    vec3 xUnit;
    vec3 yUnit;

    // Calcuate the pdf for area light.
    float calPDF(const Intersection &hit, const vec3 &toLight, const vec3 &direction) const;
};

#endif
