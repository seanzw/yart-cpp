#include "PointLight.h"

void PointLight::genShadowRay(const Hit &hit, vector<pair<Ray, float> > &rayPDFs) const {
    rayPDFs.reserve(1);
    rayPDFs.clear();
    vec3 direction = p - hit.point;
    rayPDFs.emplace_back(Ray(hit.point, normalize(direction), CONST_NEAR, length(direction)), 1.0f);
}

vec3 PointLight::Le(float t) const {
    return c;
}