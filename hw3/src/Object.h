#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Ray.h"

class BBox;
class Object {
public:
    Object() {}
    ~Object() {}

    virtual Hit intersect(const Ray &r) const = 0;
    virtual BBox getBBox() const = 0;
    virtual int intersectBBox(const BBox &box) const = 0;
};

#endif