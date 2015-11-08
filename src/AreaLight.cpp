#include "AreaLight.h"
#include "Sampler.h"

AreaLight::AreaLight(vec3 c, vec3 color, vec3 n, float r)
    : Light(color), center(c), normal(n), radius(r) {

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

pair<Ray, float> AreaLight::genShadowRay(const Intersection &hit) const {
    
    vec3 point = samplePoint();

    // Get the direction of the ray.
    vec3 toLight = point - hit.point;
    vec3 direction = normalize(toLight);

    // Get the pdf of this ray.
    float p = pdf(hit, direction);
    return make_pair(Ray(hit.point, direction, CONST_NEAR, length(toLight)), p);
}

tuple<Ray, vec3, float, float> AreaLight::genRay() const {

    // Randomly sample a point on the disk.
    vec3 point = samplePoint();

    // Randomly sample the direction.
    vec3 direction = Sampler::cosinSampleHemisphere(normal, xUnit, yUnit);

    return make_tuple(Ray(point, direction, CONST_NEAR, CONST_FAR), normal, 1.0f / area, INV_PI);
}

vec3 AreaLight::Le() const {
    return c;
}

/**
 * The pdf is the probability of projected solid angle.
 * p = (d ^ 2) / (S * cos(thetaI) * cos(thetaO));
 */
float AreaLight::pdf(const Intersection &hit, const vec3 &direction) const {

    Ray r(hit.point, direction, CONST_NEAR, CONST_FAR);

    // Intersection test with the circle.
    float dotDN = dot(normal, r.d);
    if (abs(dotDN) < CONST_NEAR) {
        return 0.0f;
    }

    float t = dot(center - r.o, normal) / dotDN;

    if (t <= CONST_NEAR) {
        return 0.0f;
    }

    float cosThetaI = clamp(dot(hit.normal, direction), 0.01f, 1.0f);
    float cosThetaO = clamp(dot(normal, -direction), 0.01f, 1.0f);
    return t * t / (area * cosThetaI * cosThetaO);
}

inline vec3 AreaLight::samplePoint() const {
    // Randomly sample a point on the disk.
    auto sample = Sampler::uniformSampleCircle();
    float r = sample.first * radius;
    float theta = sample.second;

    // Get the point on the circle.
    float x = r * cosf(theta);
    float y = r * sinf(theta);

    // Project it into world coordinate.
    vec3 point = center + x * xUnit + y * yUnit;
    return point;
}
