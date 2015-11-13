#ifndef POINT_LIGHT_HEADER
#define POINT_LIGHT_HEADER

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(vec3 color, vec3 position, vec3 attenuation) :
        Light(color), p(position), a(attenuation) {}

    virtual ~PointLight() {}

    virtual Intersection intersect(const Ray &r) const {
        return Intersection(NULL, NULL, CONST_FAR, INTERSECTION_NONE);
    }

    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const;
    virtual tuple<vec3, vec3, float> samplePoint() const;
    virtual vec3 Le() const;

private:
    vec3 p, a;
};

#endif