#include "AreaLight.h"
#include "Sampler.h"

AreaLight::AreaLight(vec3 c, vec3 color, vec3 n, float r, int nS)
    : Light(color), center(c), normal(n), radius(r), nSamples(nS) {

    area = PI * radius * radius;

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

void AreaLight::genShadowRay(const Intersection &hit, vector<pair<Ray, float> > &rayPDFs) const {
    
    rayPDFs.reserve(nSamples);
    rayPDFs.clear();
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
        vec3 toLight = point - hit.point;
        vec3 direction = normalize(toLight);

        // Get the pdf of this ray.
        float pdf = calPDF(hit, toLight, direction);
        rayPDFs.emplace_back(Ray(hit.point, direction, CONST_NEAR, length(toLight)), pdf);
    }

}

vec3 AreaLight::Le(float t) const {
    return c;
}

/**
 * The pdf is the probability projected into local coordinate, not light coordinate.
 * p = (d ^ 2) / (S * cos(thetaI) * cos(thetaO));
 */
inline float AreaLight::calPDF(const Intersection &hit, const vec3 &toLight, const vec3 &direction) const {
    float cosThetaI = clamp(dot(hit.normal, direction), 0.01f, 1.0f);
    float cosThetaO = clamp(dot(normal, -direction), 0.01f, 1.0f);
    float distanceSquared = dot(toLight, toLight);
    return distanceSquared / (area * cosThetaI * cosThetaO);
}
