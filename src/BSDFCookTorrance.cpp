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

    return color * (kd * INV_PI + ks * D * F * G / (4 * dot(normal, -in) * dot(normal, out)));
}

float BSDFCookTorrance::pdf(const Intersection &hit, const vec3 &in, const vec3 &out) const {
    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;
    vec3 wm = normalize(out - in);
    return GGX(normal, wm) / dot(wm, normal);

}

pair<Ray, float> BSDFCookTorrance::sample(const Intersection &hit, const vec3 &in) const {
    vec3 normal = hit.pos == INTERSECTION_OUT ? hit.normal : -hit.normal;

    // Sample the outgoing direction.
    float r1 = Sampler::sample1D(0.0f, 1.0f);
    float theta = atanf(roughness * sqrtf(r1 / (1.0f - r1)));
    float varphi = Sampler::sample1D(0.0f, 2.0f * PI);

    // Build the local coordinate.
    auto locals = buildLocalCoordinate(normal);

    // Transform to global coordinate.
    vec3 wm = cosf(theta) * normal +
        sinf(theta) * (locals.first * cosf(varphi) + locals.second * sinf(varphi));
    wm = normalize(wm);

    vec3 out = reflect(in, wm);

    return make_pair(Ray(hit.point, out, CONST_NEAR, CONST_FAR), pdf(hit, in, out));
}

float BSDFCookTorrance::GGX(const vec3 &n, const vec3 &wm) const {
    float cosTheta = dot(wm, n);
    float cosTheat2 = cosTheta * cosTheta;
    float tanTheat2 = (1.0f - cosTheat2) / cosTheat2;

    float root = roughness / (cosTheat2 * (roughness * roughness + tanTheat2));
    return INV_PI * (root * root);
}

float BSDFCookTorrance::Smith(const vec3 &wi, const vec3 &wo, const vec3 &wm) const {
    return SmithAux(wo, wm) * SmithAux(-wi, wm);
}

inline float BSDFCookTorrance::SmithAux(const vec3 &v, const vec3 &wm) const {
    float cosTheta = dot(wm, v);
    float cosTheat2 = cosTheta * cosTheta;
    float tanTheta2 = (1.0f - cosTheat2) / cosTheat2;
    if (tanTheta2 == 0.0f) {
        return 1.0f;
    }
    else if (cosTheta < 0.0f) {
        return 0.0f;
    }
    else {
        return 2.0f / (1.0f + sqrtf(1.0f + roughness * roughness * tanTheta2));
    }
}

float BSDFCookTorrance::SchlickFresnel(const vec3 &wi, const vec3 &wm) const {
    float oneMinusCosTheta = 1.0f - dot(-wi, wm);
    float oneMinusCosTheta2 = oneMinusCosTheta * oneMinusCosTheta;
    return r0 + (1.0f - r0) * oneMinusCosTheta2 * oneMinusCosTheta2 * oneMinusCosTheta;
}