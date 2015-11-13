#include "BRDF.h"
#include "Intersection.h"
#include "Ray.h"

float BRDF::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    return INV_PI;
}

pair<Ray, float> BRDF::sample(const Intersection &hit, const vec3 &in) const {

    // Choose the local coordinate.
    vec3 t = hit.normal.x > 0.9f ? vec3(hit.normal.y, -hit.normal.x, 0.0f) : vec3(0.0f, hit.normal.z, -hit.normal.y);
    t = normalize(t);
    vec3 s = cross(hit.normal, t);

    vec3 direction = Sampler::cosinSampleHemisphere(hit.normal, t, s);

    // Notice that we have to normalize the pdf to 1 / PI.
    return make_pair(Ray(hit.point, direction, CONST_NEAR, CONST_FAR), INV_PI);

}

vec3 BRDFSpecular::brdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    vec3 mirror = reflect(in, hit.normal);              // Find the mirror reflection direction.
    if (abs(dot(mirror, out) - 1.0f) < CONST_EPSILON) {
        // If the out direction is close to the mirror reflection.
        return specular;
    }
    else {
        return vec3(0.0f);
    }
}

float BRDFSpecular::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    return 1.0f;
}

pair<Ray, float> BRDFSpecular::sample(const Intersection &hit, const vec3 &in) const {
    vec3 mirror = reflect(in, hit.normal);
    return make_pair(Ray(hit.point, mirror, CONST_NEAR, CONST_FAR), 1.0f);
}
