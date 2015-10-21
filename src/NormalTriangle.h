#ifndef NORMAL_TRIANGLE_HEADER
#define NORMAL_TRIANGLE_HEADER

#include "Triangle.h"

class NormalTriangle : public Triangle {
public:
    NormalTriangle(int idx1, int idx2, int idx3,
        const vector<vec3> &vBuffer,
        const vector<vec3> &nBuffer
        );
    virtual ~NormalTriangle() {}

    virtual Intersection intersect(const Ray &r) const;

private:
    const vector<vec3> &n;
};

#endif