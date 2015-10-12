#include "PointLight.h"

Ray PointLight::genShadowRay(const vec3 &hit) const {
    vec3 direction = p - hit;
    return Ray(hit, normalize(direction), CONST_NEAR, length(direction));
}

vec3 PointLight::getColor(float t) const {
    float attenuation = a[0] + a[1] * t + a[2] * t * t;
    return (1.0f / attenuation) * c;
}