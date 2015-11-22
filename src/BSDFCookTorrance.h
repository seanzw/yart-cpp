#ifndef BSDF_COOK_TORRANCE_HEADER
#define BSDF_COOK_TORRANCE_HEADER

#include "BSDF.h"

/**
 * This class implements Cook-Torrance BRDF.
 *
 * Use GGX for normal distribution function.
 * Use Schlick's approximation for Fresnel term.
 * Use Smith model for shadowing term.
 */
class BSDFCookTorrance : public BSDF {
public:
    BSDFCookTorrance(const vec3 &color, float kd, float ks, float rough, float n)
        : BSDF(), color(color), kd(kd), ks(ks),
        roughness(rough), r0(((n - 1)*(n - 1)) / ((n  + 1)*(n + 1))) {}
    ~BSDFCookTorrance() {}

    virtual bool isDelta() const { return false; }
    virtual vec3 bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual float pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const;
    virtual pair<Ray, float> sample(const Intersection &hit, const vec3 &in) const;

private:
    const vec3 color;
    const float kd;
    const float ks;
    const float roughness;
    const float r0;             /* Fresnel term when theta equals 0. */

    /* For normal distribution. */
    float GGX(const vec3 &n, const vec3 &wm) const;

    /* For shadowing term. */
    float Smith(const vec3 &wi, const vec3 &wo, const vec3 &wm) const;
    inline float SmithAux(const vec3 &v, const vec3 &wm) const;

    /* For Fresnel term. */
    float SchlickFresnel(const vec3 &wi, const vec3 &wm) const;

};

#endif