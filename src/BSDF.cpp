#include "BSDF.h"
#include "Intersection.h"
#include "Ray.h"

float BSDF::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    return INV_PI;
}

pair<Ray, float> BSDF::sample(const Intersection &hit, const vec3 &in) const {

    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;

    // Choose the local coordinate.
    vec3 t = normal.x > 0.9f ? vec3(normal.y, -normal.x, 0.0f) : vec3(0.0f, normal.z, normal.y);
    t = normalize(t);
    vec3 s = cross(normal, t);

    vec3 direction = Sampler::cosinSampleHemisphere(normal, t, s);

    // Notice that we have to normalize the pdf to 1 / PI.
    return make_pair(Ray(hit.point, direction, CONST_NEAR, CONST_FAR), INV_PI);

}

vec3 BSDFLambertian::bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    return dot(normal, out) > 0.0f ? diffuse : vec3(0.0f);
}

vec3 BSDFSpecular::bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    vec3 mirror = reflect(in, normal);              // Find the mirror reflection direction.
    if (abs(dot(mirror, out) - 1.0f) < CONST_EPSILON) {
        // If the out direction is close to the mirror reflection.
        return specular;
    }
    else {
        return vec3(0.0f);
    }
}

float BSDFSpecular::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    return 1.0f;
}

pair<Ray, float> BSDFSpecular::sample(const Intersection &hit, const vec3 &in) const {
    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    vec3 mirror = reflect(in, normal);
    return make_pair(Ray(hit.point, mirror, CONST_NEAR, CONST_FAR), 1.0f);
}

vec3 BSDFRefraction::bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    vec3 r = hit.pos == INTERSECTION_OUT ? refract(in, hit.normal, inv_n) : refract(in, -hit.normal, n);
    if (glm::length(r) < CONST_EPSILON) {
        r = hit.pos == INTERSECTION_OUT ? reflect(in, hit.normal) : reflect(in, -hit.normal);
    }
    if (abs(dot(r, out) - 1.0f) < CONST_EPSILON) {
        // If the out direction is close to the mirror reflection.
        return refraction;
    }
    else {
        return vec3(0.0f);
    }
}

float BSDFRefraction::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    return 1.0f;
}

pair<Ray, float> BSDFRefraction::sample(const Intersection &hit, const vec3 &in) const {
    vec3 out = hit.pos == INTERSECTION_OUT ? refract(in, hit.normal, inv_n) : refract(in, -hit.normal, n);
    if (glm::length(out) < CONST_EPSILON) {
        out = hit.pos == INTERSECTION_OUT ? reflect(in, hit.normal) : reflect(in, -hit.normal);
    }
    return make_pair(Ray(hit.point, out, CONST_NEAR, CONST_FAR), 1.0f);
}