#include "PointLight.h"

pair<Ray, float> PointLight::genShadowRay(const Intersection &hit) const {
    vec3 direction = p - hit.point;
    return make_pair(Ray(hit.point, normalize(direction), CONST_NEAR, length(direction)), 1.0f);
}

vec3 PointLight::Le(const vec3 &point) const {
    return c;
}

tuple<vec3, vec3, float> PointLight::samplePoint() const {
    vec3 direction = Sampler::uniformSampleSphere();
    return make_tuple(p, direction, 1.0f);
}