#include "BRDF.h"
#include "Intersection.h"
#include "Ray.h"

float BRDF::pdf(const Intersection &hit, const vec3 &out) const {
    return INV_PI;
}

pair<Ray, float> BRDF::sample(const Intersection &hit) const {

    // Choose the local coordinate.
    vec3 t = hit.normal.x > 0.9f ? vec3(hit.normal.y, -hit.normal.x, 0.0f) : vec3(0.0f, hit.normal.z, -hit.normal.y);
    t = normalize(t);
    vec3 s = cross(hit.normal, t);

    vec3 direction = Sampler::cosinSampleHemisphere(hit.normal, t, s);

    // Notice that we have to normalize the pdf to 1 / PI.
    return make_pair(Ray(hit.point, direction, CONST_NEAR, CONST_FAR), INV_PI);

}
