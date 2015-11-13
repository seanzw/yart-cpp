#ifndef INTERSECTABLE_HEADER
#define INTERSECTABLE_HEADER

#include "Intersection.h"
#include "Ray.h"

class Intersectable {
public:
    Intersectable() {}
    virtual ~Intersectable() {}

    virtual Intersection intersect(const Ray &r) const = 0;
};

#endif