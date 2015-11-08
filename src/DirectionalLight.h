#ifndef DIRECTIONAL_LIGHT_HEADER
#define DIRECTIONAL_LIGHT_HEADER

#include "Light.h"

class DirectioalLight : public Light {
public:
    DirectioalLight(vec3 color, vec3 direction) : Light(color), d(direction) {}
    virtual ~DirectioalLight() {}

    inline virtual pair<Ray, float> genShadowRay(const Intersection &hit) const {
        return make_pair(Ray(hit.point, d, CONST_NEAR, CONST_FAR), 1.0f);
    }

    virtual vec3 Le() const { return c; }

private:
    vec3 d;
};

#endif
