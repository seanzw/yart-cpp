#ifndef OCTREE_HEADER
#define OCTREE_HEADER

#include "Object.h"
#include "BBox.h"

class OCTree : public Object {
public:
    OCTree(const vector<Object*> &objs,
        const BBox &boudingbox,
        const vector<int> &index,
        int level);
    ~OCTree();

    static int MAX_LEVEL;

    virtual Hit intersect(const Ray &r) const;
    virtual BBox getBBox() const { return box; }
    virtual int intersectBBox(const BBox &b) const;

private:
    bool isLeaf;
    OCTree *left;
    OCTree *right;
    const vector<Object*> &objs;
    vector<int> idx;
    BBox box;
};

#endif