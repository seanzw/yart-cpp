#include "Triangle.h"
#include "BBox.h"

Triangle::Triangle(const Material &material,
    const vec3 &vertex1, const vec3 & vertex2, const vec3 & vertex3) :
    m(material), v1(vertex1), v2(vertex2), v3(vertex3) {
    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v2;
    n = normalize(cross(edge1, edge2));
}

Hit Triangle::intersect(const Ray &r) const {
    Hit ret;

    ret.t = CONST_FAR;

    vec3 edge1 = v2 - v1;
    vec3 edge2 = v3 - v1;

    vec3 pvec = cross(r.d, edge2);
    float det = dot(edge1, pvec);

    if (abs(det) < CONST_NEAR) {
        return ret;
    } else {
        float inv_det = 1.0f / det;
        vec3 tvec = r.o - v1;
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
        ret.m = m;
        ret.normal = n;
        ret.point = r.o + ret.t * r.d;
        return ret;
    }
}

BBox Triangle::getBBox() const {
    return BBox(min(v1, min(v2, v3)) - vec3(CONST_NEAR), max(v1, max(v2, v3)) + vec3(CONST_NEAR));
}

int Triangle::intersectBBox(const BBox &box) const {
    if (box.vertexInside(v1) ||
        box.vertexInside(v2) ||
        box.vertexInside(v3)) {
        return 1;
    }

    if (box.triangleOutsideSlash(v1, v2, v3)) {
        return -1;
    }

    // edge test.
    Hit ret;

    vec3 e1 = v2 - v1;
    float t1 = length(e1);
    vec3 d1 = normalize(e1);
    ret = box.intersect(Ray(v1, d1));
    if (ret.t < t1) {
        return 1;
    }

    vec3 e2 = v3 - v1;
    float t2 = length(e2);
    vec3 d2 = normalize(e2);
    ret = box.intersect(Ray(v1, d2));
    if (ret.t < t2) {
        return 1;
    }

    vec3 e3 = v3 - v2;
    float t3 = length(e3);
    vec3 d3 = normalize(e3);
    ret = box.intersect(Ray(v2, d3));
    if (ret.t < t3) {
        return 1;
    }

    vector<Ray> diags = box.getDiagonals();
    for (const auto &diag : diags) {
        ret = intersect(diag);
        if (ret.t < CONST_FAR) {
            return 1;
        }
    }

    return -1;
}