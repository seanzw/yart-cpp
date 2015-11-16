#include "PlaneTriangle.h"

Intersection PlaneTriangle::intersect(const Ray &r) const {
    Intersection ret(NULL, this, CONST_FAR);

    vec3 edge1 = v[id2] - v[id1];
    vec3 edge2 = v[id3] - v[id1];

    vec3 pvec = cross(r.d, edge2);
    float det = dot(edge1, pvec);

    if (abs(det) < CONST_NEAR) {
        return ret;
    }
    else {
        float inv_det = 1.0f / det;
        vec3 tvec = r.o - v[id1];
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0.0f || u > 1.0f) {
            return ret;
        }

        vec3 qvec = cross(tvec, edge1);
        float v = dot(r.d, qvec) * inv_det;
        if (v < 0.0f || v > 1.0f) {
            return ret;
        }

        if (u + v > 1.0f) {
            return ret;
        }

        float t = dot(edge2, qvec) * inv_det;
        if (t < CONST_NEAR) {
            return ret;
        }

        ret.t = t;
        ret.type = INTERSECTION_OBJ;
        ret.normal = n;
        ret.point = r.o + ret.t * r.d;

        // Is the intersection inside or outside.
        ret.pos = dot(ret.normal, r.d) < 0.0f ? INTERSECTION_OUT : INTERSECTION_IN;

        return ret;
    }
}
