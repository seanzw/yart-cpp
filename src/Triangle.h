#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Object.h"
class Triangle : public Object {
public:
    Triangle(const Material &m,
        int id1, int id2, int id3,
        const vector<vec3> &vBuffer
        );
    virtual ~Triangle() {}

    virtual Hit intersect(const Ray &r) const = 0;
    virtual BBox getBBox() const;
    virtual int intersectBBox(const BBox &box) const;

    // vec3 v1, v2, v3;
	int id1, id2, id3;
    Material m;

protected:
	const vector<vec3> &v;
};

#endif