#include "AreaLight.h"
#include "Sampler.h"

AreaLight::AreaLight(vec3 c, vec3 color, vec3 n, float r, int nS)
    : Light(color), center(c), normal(n), radius(r), nSamples(nS) {

    // Build the local coordinate.
    if (abs(normal.x) > 1.0f - CONST_EPSILON) {
        xUnit = vec3(-normal.y, normal.x, 0.0f);
    }
    else {
        xUnit = vec3(0.0f, -normal.z, normal.y);
    }
    xUnit = normalize(xUnit);
    yUnit = cross(normal, xUnit);
}

void AreaLight::genShadowRay(const vec3 &hit, vector<Ray> &rays) const {
    
    rays.reserve(nSamples);
    rays.clear();
    // Randomly sample a point on the disk.
    for (int i = 0; i < nSamples; ++i) {
        pair<float, float> sample = Sampler::uniformSampleCircle();
        float r = sample.first * radius;
        float theta = sample.second;

        // Get the point on the circle.
        float x = r * cosf(theta);
        float y = r * sinf(theta);

        // Project it into world coordinate.
        vec3 point = center + x * xUnit + y * yUnit;

        // Get the direction of the ray.
        vec3 direction = point - hit;
        rays.emplace_back(hit, normalize(direction), CONST_NEAR, length(direction));
    }

}

vec3 AreaLight::getColor(float t) const {
    return c;
}
