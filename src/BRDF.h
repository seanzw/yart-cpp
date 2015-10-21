#ifndef BRDF_HEADER
#define BRDF_HEADER

#include "Const.h"

class Intersection;

class BRDF {
public:
    BRDF() {}
    virtual ~BRDF() {}
    virtual float brdf(const Intersection &hit, const vec3 &out) const = 0;
    // virtual float brdf(const Intersection &hit, const vec3 &out) = 0;

};

class BRDFLambertian : public BRDF {
public:
    BRDFLambertian() : BRDF() {}
    virtual ~BRDFLambertian() {}
    virtual float brdf(const Intersection &hit, const vec3 &out) const {
        return INV_PI;
    }
};

#endif