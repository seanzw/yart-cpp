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

    /* Is this BRDF a delta function? */
    virtual bool isDelta() const = 0;

    /**
     * hit: intersection.
     * in : income direction.
     * out: outcome direction.
     * Return the brdf.
     */
    virtual vec3 brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const = 0;

    /**
     * Return the pdf of projected solid angle of sampling this direction.
     * Default the cosin pdf.
     */
    virtual float pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;

    /**
     * Return one sample ray along with the pdf.
     * Default sampling with cosin pdf.
     * Return Ray: the outgoing ray.
     * Return float: the possibility of the projected solid angle.
     */
    virtual pair<Ray, float> sample(const Intersection &hit, const vec3 &in) const;
};

/**
 * Perfect diffuse BRDF.
 */
class BRDFLambertian : public BRDF {
public:
    BRDFLambertian(const vec3 &d) : BRDF(), diffuse(d * INV_PI) {}
    virtual ~BRDFLambertian() {}
    virtual bool isDelta() const { return false; }
    virtual vec3 brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
        return diffuse;
    }
    const vec3 diffuse;
};

/**
 * Perfect specular BRDF.
 *
 * Notice that perfect specular BRDF contains a delta function in the BRDF and pdf.
 * The return value of these two functions are just coefficients of a delta function.
 */
class BRDFSpecular : public BRDF {
public:
    BRDFSpecular(const vec3 &s) : BRDF(), specular(s) {}
    virtual ~BRDFSpecular() {}
    virtual bool isDelta() const { return true; }
    virtual vec3 brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual float pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual pair<Ray, float> sample(const Intersection &hit, const vec3 &in) const;

    const vec3 specular;
};

#endif