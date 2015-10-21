#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Ray.h"
#include "Intersection.h"

class BBox;
class Object {
public:
    Object() {}
    virtual ~Object() {}

    virtual bool occlude(const Ray &r) const = 0;
    virtual Intersection intersect(const Ray &r) const = 0;
    virtual BBox getBBox() const = 0;
    virtual int intersectBBox(const BBox &box) const = 0;
};

#endif
