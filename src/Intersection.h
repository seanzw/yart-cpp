#ifndef INTERSECTION_HEADER
#define INTERSECTION_HEADER

#include "Material.h"

class Intersection {
public:

    Intersection(const Material *material, float t = CONST_FAR) : m(material), t(t) {}
    ~Intersection() {}

    float t;
    vec3 point;
    vec3 normal;
    const Material *m;

};


#endif