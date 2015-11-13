#ifndef INTERSECTION_HEADER
#define INTERSECTION_HEADER

#include "Material.h"

enum IntersectionType {
    INTERSECTION_OBJ,
    INTERSECTION_LIGHT,
    INTERSECTION_NONE
};

class Intersectable;
class Intersection {
public:

    Intersection(const Material *material,
        const Intersectable *intersectable,
        float t = CONST_FAR,
        IntersectionType type = INTERSECTION_NONE
        ) : m(material), intersectable(intersectable), t(t), type(type) {}
    ~Intersection() {}

    float t;
    vec3 point;
    vec3 normal;
    const Material *m;
    const Intersectable *intersectable;
    
    IntersectionType type;

};


#endif