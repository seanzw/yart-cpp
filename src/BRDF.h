#ifndef BRDF_HEADER
#define BRDF_HEADER

#include "Const.h"
#include "Sampler.h"

class Intersection;
class Ray;

class BRDF {
public:
    BRDF() {}
    virtual ~BRDF() {}

    /**
     * hit: intersection.
     * in : income direction.
     * out: outcome direction.
     * Return the brdf.
     */
    virtual vec3 brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const = 0;

    /**
     * Return the pdf of projected solid angle.
     * Default the cosin pdf.
     */
    virtual float pdf(const Intersection &hit, const vec3 &out) const;

    /**
     * Return one sample ray along with the pdf.
     * Default sampling with cosin pdf.
     * Return Ray: the outgoing ray.
     * Return float: the possibility of the projected solid angle.
     */
    virtual pair<Ray, float> sample(const Intersection &hit) const;
};

class BRDFLambertian : public BRDF {
public:
    BRDFLambertian(const vec3 &d) : BRDF(), diffuse(d) {}
    virtual ~BRDFLambertian() {}
    virtual vec3 brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
        return vec3(INV_PI) * diffuse;
    }
    const vec3 diffuse;
};

#endif