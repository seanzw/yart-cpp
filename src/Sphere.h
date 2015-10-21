#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Object.h"

class Sphere : public Object {
public:
    Sphere(const Material &material,
        const vec3 &center, float radius,
        const mat4 &trans);
    virtual ~Sphere() {}

    virtual Intersection intersect(const Ray &r) const;
    virtual bool occlude(const Ray &r) const;
    virtual BBox getBBox() const;
    virtual int intersectBBox(const BBox &box) const;

private:
    vec3 c;
    float r;
    mat4 t;
    mat4 inv_t;
    mat3 normal_mat;
    Material m;
};

#endif