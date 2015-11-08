#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include <tuple>
#include "Ray.h"
#include "Intersection.h"

class Light {
public:
    Light(vec3 color): c(color) {}
    virtual ~Light() {}

    /* Sample the shadow rays along with their pdfs. */
    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const = 0;

    /**
     * Sample a ray from the lignt.
     * Return Ray: the ray left the light.
     * Return vec3: the normal at this point.
     * Return float: the possibility of choosing the origin.
     * Return float: the possibility of choosing the direction, projected solid angle.
     */
    virtual tuple<Ray, vec3, float, float> genRay() const {
        cerr << "Light::genRay() unimplemented.\n";
        exit(1);
    }

    virtual float pdf(const Intersection &hit, const vec3 &direction) const {
        cerr << "light::pdf() unimplemented.\n";
        exit(1);
    }
    virtual vec3 Le() const = 0;

protected:
    vec3 c;
};

#endif