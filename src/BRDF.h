#ifndef BRDF_HEADER
#define BRDF_HEADER

#include "Const.h"

class BRDF {
public:
    BRDF() {}
    virtual ~BRDF() {}

    virtual float brdf(const vec3 &in, const vec3 &out, const vec3 &normal, const vec3 &tangent) = 0;

};

class BRDFLambertian : public BRDF {
public:
    BRDFLambertian() : BRDF() {}
    virtual ~BRDFLambertian() {}
    virtual float brdf(const vec3 &in, const vec3 &out, const vec3 &normal, const vec3 &tangent) {
        return INV_PI;
    }
};

#endif