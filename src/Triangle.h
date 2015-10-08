#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Object.h"
class Triangle : public Object {
public:
    Triangle(const Material &m,
        const vec3 & vertex1, const vec3 & vertex2, const vec3 & vertex3
        );
    virtual ~Triangle() {}

    virtual Hit intersect(const Ray &r) const;
    virtual BBox getBBox() const;
    virtual int intersectBBox(const BBox &box) const;

private:
    vec3 v1, v2, v3;
    vec3 n;
    Material m;
};

#endif