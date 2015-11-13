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

    virtual Intersection intersect(const Ray &r) const;

    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const;
    virtual tuple<vec3, vec3, float> samplePoint() const;
    virtual float pdfPoint(const vec3 &point) const {
        return 1.0f / area;
    }
    virtual float pdfRay(const Intersection &hit, const vec3 &direction) const;
    virtual vec3 Le() const;

private:
    vec3 center;
    vec3 normal;
    float radius;
    float area;

    // Local coordinate.
    vec3 xUnit;
    vec3 yUnit;
};

#endif
