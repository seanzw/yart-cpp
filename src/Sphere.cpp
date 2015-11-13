#include "Sphere.h"
#include "BBox.h"

#include "glm\gtc\matrix_inverse.hpp"

Sphere::Sphere(const Material &material,
    const vec3 &center, float radius,
    const mat4 &trans) : m(material), c(center), r(radius), t(trans) {
    inv_t = inverse(t);
    normal_mat = inverseTranspose(mat3(t));
}

Intersection Sphere::intersect(const Ray &ray) const {

    // Inverse transform the ray.
    vec3 o = applyMatrix(inv_t, ray.o);
    vec3 d = normalize(applyMatrix(inv_t, ray.o + ray.d) - o);

    Intersection ret(&m, this, CONST_FAR);

    float r2 = r * r;
    vec3 toSphere = c - o;
    float l2 = dot(toSphere, toSphere);

    if (l2 > r2) {
        float d2 = dot(toSphere, d);
        if (d2 <= 0.0f) {
            return ret;
        }

        float thc = r2 - l2 + d2 * d2;
        if (thc <= 0.0f) {
            return ret;
        }

        float thc_sqrt = sqrtf(thc);
        float t_temp = d2 - thc_sqrt;
        if (t_temp > CONST_NEAR) {
            vec3 hitpoint = o + t_temp * d;
            vec3 normal = normalize(hitpoint - c);
            ret.point = applyMatrix(t, hitpoint);
            ret.t = length(ret.point - ray.o);
            ret.normal = normalize(normal_mat * normal);
            ret.type = INTERSECTION_OBJ;
        }
        else {
            t_temp = d2 + thc_sqrt;
            if (t_temp > CONST_NEAR) {
                vec3 hitpoint = o + t_temp * d;
                vec3 normal = normalize(hitpoint - c);
                ret.point = applyMatrix(t, hitpoint);
                ret.t = length(ret.point - ray.o);
                ret.normal = normalize(normal_mat * normal);
                ret.type = INTERSECTION_OBJ;
            }
        }
        return ret;
    }
    else {
        float d2 = dot(toSphere, d);
        float thc = r2 - l2 + d2 * d2;
        float t_temp = sqrtf(thc) + d2;
        if (t_temp > CONST_NEAR) {
            vec3 hitpoint = o + t_temp * d;
            vec3 normal = normalize(hitpoint - c);
            ret.point = applyMatrix(t, hitpoint);
            ret.t = length(ret.point - ray.o);
            ret.normal = normalize(normal_mat * normal);
            ret.type = INTERSECTION_OBJ;
        }
        return ret;
    }
}

bool Sphere::occlude(const Ray &ray) const {
    vec3 o = applyMatrix(inv_t, ray.o);
    vec3 d = normalize(applyMatrix(inv_t, ray.o + ray.d) - o);

    float r2 = r * r;
    vec3 toSphere = c - o;
    float l2 = dot(toSphere, toSphere);

    if (l2 > r2) {
        float d2 = dot(toSphere, d);
        if (d2 <= 0.0f) {
            return false;
        }

        float thc = r2 - l2 + d2 * d2;
        if (thc <= 0.0f) {
            return false;
        }

        float thc_sqrt = sqrtf(thc);
        float t_temp = d2 - thc_sqrt;
        if (t_temp > ray.tmin) {
            return t_temp < ray.tmax;
        }
        else {
            t_temp = d2 + thc_sqrt;
            return t_temp > ray.tmin && t_temp < ray.tmax;
        }
    }
    else {
        float d2 = dot(toSphere, d);
        float thc = r2 - l2 + d2 * d2;
        float t_temp = sqrtf(thc) + d2;
        return t_temp > ray.tmin && t_temp < ray.tmax;
    }
}

BBox Sphere::getBBox() const {
    return BBox(c - vec3(r), c + vec3(r));
}

int Sphere::intersectBBox(const BBox &box) const {
    return getBBox().intersectBBox(box);
}