#ifndef NORMAL_TRIANGLE_HEADER
#define NORMAL_TRIANGLE_HEADER

#include "Object.h"

class NormalTriangle : public Object {
public:
    NormalTriangle(const Material &m,
        int id1, int id2, int id3,
        const vector<vec3> &vBuffer,
        const vector<vec3> &nBuffer
        );
    virtual ~NormalTriangle() {}

    virtual Hit intersect(const Ray &r) const;
    virtual BBox getBBox() const;
    virtual int intersectBBox(const BBox &box) const;

private:

    int id1, id2, id3;
    const vector<vec3> &v;
    const vector<vec3> &n;
    Material m;
};

#endif