#include "PointLight.h"

pair<Ray, float> PointLight::genShadowRay(const Intersection &hit) const {
    vec3 direction = p - hit.point;
    return make_pair(Ray(hit.point, normalize(direction), CONST_NEAR, length(direction)), 1.0f);
}

vec3 PointLight::Le() const {
    return c;
}

tuple<Ray, vec3, float, float> PointLight::genRay() const {
    vec3 direction = Sampler::uniformSampleSphere();
    return make_tuple(Ray(p, direction, CONST_NEAR, CONST_FAR), direction, 1.0f, INV_PI);
}