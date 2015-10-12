#ifndef NORMAL_TRIANGLE_HEADER
#define NORMAL_TRIANGLE_HEADER

#include "Triangle.h"

class NormalTriangle : public Triangle {
public:
    NormalTriangle(const Material &m,
        int idx1, int idx2, int idx3,
        const vector<vec3> &vBuffer,
        const vector<vec3> &nBuffer
        );
    virtual ~NormalTriangle() {}

    virtual Hit intersect(const Ray &r) const;

private:
    const vector<vec3> &n;
};

#endif