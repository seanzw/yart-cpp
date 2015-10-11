#include "Mesh.h"
#include "BBox.h"

Hit Mesh::intersect(const Ray &r) const {
	Hit ret;
	ret.t = CONST_FAR;
	if (useTree) {
		ret = tree->intersect(r);
	}
	else {
		for (const auto &face : f) {
			Hit temp = face->intersect(r);
			if (temp.t < ret.t) {
				ret = temp;
			}
		}
	}
	return ret;
}

BBox Mesh::getBBox() const {
	BBox ret = f[0]->getBBox();
	for (const auto &face : f) {
		ret = ret.merge(face->getBBox());
	}
	return ret;
}

int Mesh::intersectBBox(const BBox &box) const {
	for (const auto &face : f) {
		if (face->intersectBBox(box) == 1) {
			return 1;
		}
	}
	return -1;
}

void Mesh::pushVertex(const vec3 &vertex) {
	v.push_back(vertex);
}

void Mesh::pushTri(const Material &m, int id1, int id2, int id3) {

	f.push_back(move(make_unique<Triangle>(m, id1, id2, id3, v)));
}

void Mesh::buildOCTree(int level) {
	OCTree::MAX_LEVEL = level > 1 ? level : 1;
	BBox b = getBBox();
	vector<int> idx;
	for (unsigned int i = 0; i < f.size(); ++i) {
		idx.push_back(i);
	}
	tree = std::make_unique<OCTree>(f, b, idx, 0);
	useTree = true;
}