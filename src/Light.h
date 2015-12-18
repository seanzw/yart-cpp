#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include <tuple>
#include "Intersectable.h"

/**
 * Light is moduled as a special object.
 */
class Light : public Intersectable {
public:
    Light() {}
    virtual ~Light() {}

    /* Interface from Ojbect. */
    virtual Intersection intersect(const Ray &r) const = 0;

    /* Sample the shadow rays along with their pdfs. */
    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const = 0;

    /**
     * Sample a point on the lignt.
     *
     * @retrun vec3: the point on the light
     * @retrun vec3: the normal at this point.
     * @retrun float: the possibility of sampling this point.
     */
    virtual tuple<vec3, vec3, float> samplePoint() const {
        cerr << "Light::samplePoint() unimplemented.\n";
        exit(1);
    }

    /**
     * The probability that this point is sampled.
     */
    virtual float pdfPoint(const vec3 &point) const {
        cerr << "light::pdfPoint() unimplemented.\n";
        exit(1);
    }

    /**
     * The probability that this ray is sampled.
     */
    virtual float pdfRay(const Intersection &hit, const vec3 &direction) const {
        cerr << "light::pdfRay() unimplemented.\n";
        exit(1);
    }

    /* Returns the Le of the light along normal direction. */
    virtual vec3 Le(const vec3 &point) const = 0;
};

#endif