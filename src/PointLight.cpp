#include "PointLight.h"

void PointLight::genShadowRay(const vec3 &hit, unique_ptr<vector<Ray> > &rays) const {
    rays->reserve(1);
    rays->clear();
    vec3 direction = p - hit;
    rays->emplace_back(hit, normalize(direction), CONST_NEAR, length(direction));
}

vec3 PointLight::getColor(float t) const {
    float attenuation = a[0] + a[1] * t + a[2] * t * t;
    return (1.0f / attenuation) * c;
}