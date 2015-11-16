#ifndef BSDF_HEADER
#define BSDF_HEADER

#include "Const.h"
#include "Sampler.h"

class Intersection;
class Ray;

class BSDF {
public:
    BSDF() {}
    virtual ~BSDF() {}

    /* Is this BSDF a delta function? */
    virtual bool isDelta() const = 0;

    /**
     * hit: intersection.
     * in : income direction.
     * out: outcome direction.
     * Return the BSDF.
     */
    virtual vec3 bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const = 0;

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
 * Perfect diffuse BSDF.
 */
class BSDFLambertian : public BSDF {
public:
    BSDFLambertian(const vec3 &d) : BSDF(), diffuse(d * INV_PI) {}
    virtual ~BSDFLambertian() {}
    virtual bool isDelta() const { return false; }
    virtual vec3 bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    const vec3 diffuse;
};

/**
 * Perfect specular BSDF.
 *
 * Notice that perfect specular BSDF contains a delta function in the BSDF and pdf.
 * The return value of these two functions are just coefficients of a delta function.
 */
class BSDFSpecular : public BSDF {
public:
    BSDFSpecular(const vec3 &s) : BSDF(), specular(s) {}
    virtual ~BSDFSpecular() {}
    virtual bool isDelta() const { return true; }
    virtual vec3 bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual float pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual pair<Ray, float> sample(const Intersection &hit, const vec3 &in) const;

    const vec3 specular;
};

/**
 * Perfect refraction BSDF.
 *
 * This BSDF also contains a delta function like specular BSDF.
 */
class BSDFRefraction : public BSDF {
public:
    BSDFRefraction(const vec3 &r, float n) : BSDF(), refraction(r), n(n), inv_n(1.0f / n) {}
    virtual ~BSDFRefraction() {}
    virtual bool isDelta() const { return true; }
    virtual vec3 bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual float pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual pair<Ray, float> sample(const Intersection &hit, const vec3 &in) const;

    const vec3 refraction;
    const float n;
    const float inv_n;
};

#endif