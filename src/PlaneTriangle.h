#ifndef PLANE_TRIANGLE_HEADER
#define PLANE_TRIANGLE_HEADER

#include "Triangle.h"

class PlaneTriangle : public Triangle {
public:
    PlaneTriangle(const Material &m,
        int id1, int id2, int id3,
        const vector<vec3> &vBuffer
        ) : Triangle(m, id1, id2, id3, vBuffer) {
        vec3 edge1 = v[id2] - v[id1];
        vec3 edge2 = v[id3] - v[id2];
        n = normalize(cross(edge1, edge2));
    }

    virtual ~PlaneTriangle() {}

    virtual Hit intersect(const Ray &r) const;

private:
    vec3 n;
};

#endif