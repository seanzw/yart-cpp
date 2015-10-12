#include "PointLight.h"

Ray PointLight::genShadowRay(const vec3 &hit, float *t_out) {
    vec3 direction = p - hit;
    *t_out = length(direction);
    return Ray(hit, normalize(direction), CONST_NEAR, CONST_FAR);
}

vec3 PointLight::getColor(float t) {
    float attenuation = a[0] + a[1] * t + a[2] * t * t;
    return (1.0f / attenuation) * c;
}