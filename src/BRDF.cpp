#include "BRDF.h"
#include "Intersection.h"
#include "Ray.h"

float BRDF::pdf(const Intersection &hit, const vec3 &out) const {
    return dot(hit.normal, out);
}

pair<Ray, float> BRDF::sample(const Intersection &hit) const {

    // Choose the local coordinate.
    vec3 t = hit.normal.x > 0.9f ? vec3(hit.normal.y, -hit.normal.x, 0.0f) : vec3(0.0f, hit.normal.z, -hit.normal.y);
    t = normalize(t);
    vec3 s = cross(hit.normal, t);

    // Sample in a circle.
    auto sample = Sampler::uniformSampleCircle();

    // Transform it into the hemisphere.
    float pdf = sqrtf(1.0f - sample.first * sample.first);
    vec3 direction = (t * cos(sample.second) + s * sin(sample.second)) * sample.first + hit.normal * pdf;
    direction = normalize(direction);

    return make_pair(Ray(hit.point, direction, CONST_NEAR, CONST_FAR), pdf);

}
