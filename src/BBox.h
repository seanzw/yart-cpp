#ifndef BBOX_HEADER
#define BBOX_HEADER

#include "Object.h"

class BBox : public Object {
public:
    BBox(const vec3 &minimum, const vec3 &maximum) :
        m_min(minimum), m_max(maximum) {}
    BBox() :
        m_min(vec3(0.0f)), m_max(vec3(0.0f)) {}
    ~BBox() {}

    virtual Intersection intersect(const Ray &r) const;
    virtual bool occlude(const Ray &r) const;
    virtual BBox getBBox() const { return BBox(m_min, m_max); }
    virtual int intersectBBox(const BBox &other) const;
    pair<BBox, BBox> split(int axis, float ratio = 0.5f) const;
    vector<Ray> getDiagonals() const;
    bool vertexInside(const vec3 &v) const;
    bool triangleOutsideSlash(const vec3 &v1, const vec3 &v2, const vec3 &v3) const;
    inline BBox merge(const BBox &other) const {
        return BBox(min(m_min, other.m_min), max(m_max, other.m_max));
    }
private:
    vec3 m_min, m_max;
};

#endif