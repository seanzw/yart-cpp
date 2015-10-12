#ifndef MESH_HEADER
#define MESH_HEADER

#include "Object.h"
#include "Triangle.h"
#include "OCTree.h"

class Mesh : public Object {
public:
	Mesh() {
		useTree = false;
	}
	virtual ~Mesh() {}

	virtual Hit intersect(const Ray &r) const;
	virtual BBox getBBox() const;
	virtual int intersectBBox(const BBox &box) const;

	void pushVertex(const vec3 &vertex);
	void pushTri(const Material &m, int id1, int id2, int id3);
    void refine();
	void buildOCTree(int level);

private:
	vector<unique_ptr<Object> > f;
	vector<vec3> v;
	vector<vec3> n;
	unique_ptr<OCTree> tree;
	bool useTree;
};


#endif
