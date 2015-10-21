#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Ray.h"
#include "Intersection.h"

class Light {
public:
    Light(vec3 color): c(color) {}
    virtual ~Light() {}

    /* Sample the shadow rays along with their pdfs. */
    virtual void genShadowRay(const Intersection &hit, vector<pair<Ray, float> > &rayPDFs) const = 0;
    virtual vec3 Le(float t) const = 0;

protected:
    vec3 c;
};

#endif