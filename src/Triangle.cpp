#include "Triangle.h"
#include "BBox.h"

Triangle::Triangle(const Material &material,
	int idx1, int idx2, int idx3,
	const vector<vec3> &vBuffer
    ) : m(material), v(vBuffer), id1(idx1), id2(idx2), id3(idx3) {

}

BBox Triangle::getBBox() const {
    return BBox(min(v[id1], min(v[id2], v[id3])) - vec3(CONST_NEAR), max(v[id1], max(v[id2], v[id3])) + vec3(CONST_NEAR));
}

int Triangle::intersectBBox(const BBox &box) const {
    if (box.vertexInside(v[id1]) ||
        box.vertexInside(v[id2]) ||
        box.vertexInside(v[id3])) {
        return 1;
    }

    if (box.triangleOutsideSlash(v[id1], v[id2], v[id3])) {
        return -1;
    }

    // edge test.
    Hit ret;

    vec3 e1 = v[id2] - v[id1];
    float t1 = length(e1);
    vec3 d1 = normalize(e1);
    ret = box.intersect(Ray(v[id1], d1, CONST_NEAR, CONST_FAR));
    if (ret.t < t1) {
        return 1;
    }

    vec3 e2 = v[id3] - v[id1];
    float t2 = length(e2);
    vec3 d2 = normalize(e2);
    ret = box.intersect(Ray(v[id1], d2, CONST_NEAR, CONST_FAR));
    if (ret.t < t2) {
        return 1;
    }

    vec3 e3 = v[id3] - v[id2];
    float t3 = length(e3);
    vec3 d3 = normalize(e3);
    ret = box.intersect(Ray(v[id2], d3, CONST_NEAR, CONST_FAR));
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

bool Triangle::occlude(const Ray &r) const {

    vec3 edge1 = v[id2] - v[id1];
    vec3 edge2 = v[id3] - v[id1];

    vec3 pvec = cross(r.d, edge2);
    float det = dot(edge1, pvec);

    if (abs(det) < CONST_NEAR) {
        return false;
    }
    else {
        float inv_det = 1.0f / det;
        vec3 tvec = r.o - v[id1];
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        vec3 qvec = cross(tvec, edge1);
        float v = dot(r.d, qvec) * inv_det;
        if (v < 0.0f || v > 1.0f) {
            return false;
        }

        if (u + v > 1.0f) {
            return false;
        }

        float t = dot(edge2, qvec) * inv_det;
        if (t < r.tmin || t > r.tmax) {
            return false;
        }

        return true;
    }
}