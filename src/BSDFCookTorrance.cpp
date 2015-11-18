#include "BSDFCookTorrance.h"
#include "Intersection.h"
#include "Ray.h"

vec3 BSDFCookTorrance::bsdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {

    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    if (dot(normal, out) < 0.0f) {
        return vec3(0.0f);
    }

    // Calculate the microface normal.
    vec3 wm = normalize(-in + out);

    float D = GGX(normal, wm);
    float G = Smith(in, out, wm);
    float F = SchlickFresnel(in, wm);

    return color * D * F * G / (4 * dot(normal, -in) * dot(normal, out));
}

float BSDFCookTorrance::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    // return INV_PI;
    return BSDF::pdf(hit, in, out);
}

pair<Ray, float> BSDFCookTorrance::sample(const Intersection &hit, const vec3 &in) const {
    // vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    // vec3 mirror = reflect(in, normal);
    // return make_pair(Ray(hit.point, mirror, CONST_NEAR, CONST_FAR), 1.0f);
    return BSDF::sample(hit, in);
}

float BSDFCookTorrance::GGX(const vec3 &n, const vec3 &wm) const {
    float cosTheta = dot(wm, n);
    float cosTheat2 = cosTheta * cosTheta;
    vec3 sinEdge = wm - n * cosTheta;
    float sinTheta2 = dot(sinEdge, sinEdge);
    float tanTheat2 = sinTheta2 / cosTheat2;

    float root = roughness / (cosTheat2 * (roughness * roughness + tanTheat2));
    return INV_PI * (root * root);
}

float BSDFCookTorrance::Smith(const vec3 &wi, const vec3 &wo, const vec3 &wm) const {
    return SmithAux(wo, wm) * SmithAux(-wi, wm);
}

inline float BSDFCookTorrance::SmithAux(const vec3 &v, const vec3 &wm) const {
    float cosTheta = dot(wm, v);
    float sinTheta = length(v - wm * cosTheta);
    float tanTheta = abs(sinTheta / cosTheta);
    if (tanTheta == 0.0f) {
        return 1.0f;
    }
    else if (cosTheta < 0.0f) {
        return 0.0f;
    }
    else {
        float root = roughness * tanTheta;
        return 2.0f / (1.0f + sqrtf(1.0f + root * root));
    }
}

float BSDFCookTorrance::SchlickFresnel(const vec3 &wi, const vec3 &wm) const {
    float oneMinusCosTheta = 1.0f - dot(-wi, wm);
    float oneMinusCosTheta2 = oneMinusCosTheta * oneMinusCosTheta;
    return r0 + (1.0f - r0) * oneMinusCosTheta2 * oneMinusCosTheta2 * oneMinusCosTheta;
}