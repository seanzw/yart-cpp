#include "BBox.h"

Intersection BBox::intersect(const Ray &r) const {

    Intersection ret(NULL, CONST_FAR);

    vec3 tMin = (m_min - r.o) / r.d;
    vec3 tMax = (m_max - r.o) / r.d;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(t1[0], max(t1[1], t1[2]));
    float tFar = min(t2[0], min(t2[1], t2[2]));

    if (tNear > tFar) {
        return ret;
    }
    else {
        if (tNear > 0.0f) {
            ret.t = tNear;
        }
        else if (tFar > 0.0f) {
            ret.t = tFar;
        }
        else {
            ret.t = CONST_FAR;
        }
        return ret;
    }
}

bool BBox::occlude(const Ray &r) const {

    vec3 tMin = (m_min - r.o) / r.d;
    vec3 tMax = (m_max - r.o) / r.d;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(t1[0], max(t1[1], t1[2]));
    float tFar = min(t2[0], min(t2[1], t2[2]));

    if (tNear > tFar) {
        return false;
    }
    else {
        if (tNear > 0.0f) {
            return true;
        }
        else if (tFar > 0.0f) {
            return true;
        }
        else {
            return false;
        }
    }
}

pair<BBox, BBox> BBox::split(int axis, float ratio) const {
    vec3 min_new(m_min);
    vec3 max_new(m_max);
    switch (axis)
    {
    case 0: {
        float x_delta = m_max[0] - m_min[0];
        max_new[0] = min_new[0] = m_min[0] + ratio * x_delta;
        break;
    }
    case 1: {
        float y_delta = m_max[1] - m_min[1];
        max_new[1] = min_new[1] = m_min[1] + ratio * y_delta;
        break;
    }
    case 2: {
        float z_delta = m_max[2] - m_min[2];
        max_new[2] = min_new[2] = m_min[2] + ratio * z_delta;
        break;
    }
    default: {
        cerr << "ERROR: split axis must be 0, 1, 2" << endl;
        exit(1);
        break;
    }
    }
    BBox box1(m_min, max_new);
    BBox box2(min_new, m_max);
    return make_pair(box1, box2);
}

int BBox::intersectBBox(const BBox &other) const {

    if (other.m_min.x > m_max.x ||
        other.m_max.x < m_min.x) {
        return -1;
    }

    if (other.m_min.y > m_max.y ||
        other.m_max.y < m_min.y) {
        return -1;
    }

    if (other.m_min.z > m_max.z ||
        other.m_max.z < m_min.z) {
        return -1;
    }

    return 1;
}

bool BBox::vertexInside(const vec3 &v) const {
    if (v.x >= m_min.x && v.x <= m_max.x &&
        v.y >= m_min.y && v.y <= m_max.y &&
        v.z >= m_min.z && v.z <= m_max.z) {
        return true;
    }
    return false;
}
bool BBox::triangleOutsideSlash(const vec3 &v1, const vec3 &v2, const vec3 &v3) const {
    if (v1.x < m_min.x &&
        v2.x < m_min.x &&
        v3.x < m_min.x) {
        return true;
    }

    if (v1.x > m_max.x &&
        v2.x > m_max.x &&
        v3.x > m_max.x) {
        return true;
    }

    if (v1.y < m_min.y &&
        v2.y < m_min.y &&
        v3.y < m_min.y) {
        return true;
    }

    if (v1.y > m_max.y &&
        v2.y > m_max.y &&
        v3.y > m_max.y) {
        return true;
    }

    if (v1.z < m_min.z &&
        v2.z < m_min.z &&
        v3.z < m_min.z) {
        return true;
    }

    if (v1.z > m_max.z &&
        v2.z > m_max.z &&
        v3.z > m_max.z) {
        return true;
    }

    return false;
}

vector<Ray> BBox::getDiagonals() const {
    vector<Ray> ret;
    vec3 o;
    vec3 e;
    vec3 d;

    o = m_min;
    e = m_max;
    d = normalize(e - o);
    ret.push_back(Ray(o, d, CONST_NEAR, CONST_FAR));

    o = vec3(m_max.x, m_min.y, m_min.z);
    e = vec3(m_min.x, m_max.y, m_max.z);
    d = normalize(e - o);
    ret.push_back(Ray(o, d, CONST_NEAR, CONST_FAR));

    o = vec3(m_min.x, m_max.y, m_min.z);
    e = vec3(m_max.x, m_min.y, m_max.z);
    d = normalize(e - o);
    ret.push_back(Ray(o, d, CONST_NEAR, CONST_FAR));

    o = vec3(m_min.x, m_min.y, m_max.z);
    e = vec3(m_max.x, m_max.y, m_min.z);
    d = normalize(e - o);
    ret.push_back(Ray(o, d, CONST_NEAR, CONST_FAR));

    return ret;
}