#include "OCTree.h"
#include <cstdlib>
#define OCTREE_MAX_LEVEL 2

OCTree::OCTree(const vector<Object*> &objects,
    const BBox &boudingbox,
    const vector<int> &index,
    int level
    ) : objs(objects), box(boudingbox) {

    if (level == OCTREE_MAX_LEVEL) {
        // Leaf.
        isLeaf = true;
        for (const auto i : index) {
            idx.push_back(i);
        }
    }
    else {
        // Node.
        isLeaf = false;
        // Randomly split the bbox.
        int axis = rand() % 3;
        auto childboxes = box.split(axis);

        // Left child
        vector<int> left_idx;
        BBox left_box = childboxes.first;
        for (const auto id : index) {
            if (objs[id]->intersectBBox(left_box) >= 0) {
                left_idx.push_back(id);
            }
        }
        left = new OCTree(objs, left_box, left_idx, level + 1);

        vector<int> right_idx;
        BBox right_box = childboxes.first;
        for (const auto id : index) {
            if (objs[id]->intersectBBox(right_box) >= 0) {
                right_idx.push_back(id);
            }
        }
        right = new OCTree(objs, right_box, right_idx, level + 1);
    }
}

OCTree::~OCTree() {
    if (!isLeaf) {
        delete left;
        delete right;
    }
}

int OCTree::intersectBBox(const BBox &b) const {
    return box.intersectBBox(b);
}

Hit OCTree::intersect(const Ray &r) const {
    Hit ret;
    ret = box.intersect(r);
    if (ret.t >= CONST_FAR) {
        ret.t = CONST_FAR;
        return ret;
    }
    ret.t = CONST_FAR;
    if (isLeaf) {
        for (const auto id : idx) {
            Hit temp = objs[id]->intersect(r);
            if (temp.t < ret.t) {
                ret = temp;
            }
        }
        return ret;
    }
    else {
        Hit leftHit = left->intersect(r);
        Hit rightHit = right->intersect(r);
        if (leftHit.t < rightHit.t) {
            return leftHit;
        }
        else {
            return rightHit;
        }
    }
}