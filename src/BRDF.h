#ifndef BRDF_HEADER
#define BRDF_HEADER

#include "Const.h"

class Intersection;

class BRDF {
public:
    BRDF() {}
    virtual ~BRDF() {}
    virtual vec3 brdf(const Intersection &hit, const vec3 &out) const = 0;
};

class BRDFLambertian : public BRDF {
public:
    BRDFLambertian(const vec3 &d) : BRDF(), diffuse(d) {}
    virtual ~BRDFLambertian() {}
    virtual vec3 brdf(const Intersection &hit, const vec3 &out) const {
        return vec3(INV_PI) * diffuse;
    }
    const vec3 diffuse;
};

#endif