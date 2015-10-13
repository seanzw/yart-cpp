#ifndef DIRECTIONAL_LIGHT_HEADER
#define DIRECTIONAL_LIGHT_HEADER

#include "Light.h"

class DirectioalLight : public Light {
public:
    DirectioalLight(vec3 color, vec3 direction) : Light(color), d(direction) {}
    virtual ~DirectioalLight() {}

    inline virtual void genShadowRay(const vec3 &hit, unique_ptr<vector<Ray> > &rays) const {
        rays->reserve(1);
        rays->clear();
        rays->emplace_back(hit, d, CONST_NEAR, CONST_FAR);
    }

    virtual vec3 getColor(float t) const { return c; }

private:
    vec3 d;
};

#endif
