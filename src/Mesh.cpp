#include "Mesh.h"
#include "BBox.h"
#include "PlaneTriangle.h"
#include "NormalTriangle.h"

Intersection Mesh::intersect(const Ray &r) const {
    Intersection ret(NULL, this, CONST_FAR);
	if (useTree) {
		ret = tree->intersect(r);
	}
	else {
		for (const auto &face : f) {
            Intersection temp = face->intersect(r);
			if (temp.t < ret.t) {
				ret = temp;
			}
		}
	}
    ret.m = &material;
    return ret;
}

bool Mesh::occlude(const Ray &r) const {
    if (useTree) {
        return tree->occlude(r);
    }
    else {
        for (const auto &face : f) {
            if (face->occlude(r)) {
                return true;
            }
        }
    }
    return false;
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

void Mesh::pushTri(int id1, int id2, int id3) {

	f.push_back(move(make_unique<PlaneTriangle>(id1, id2, id3, v)));
}

/* Refine the mesh by calculate the normal for each vertex and use NormalTriangle. */
void Mesh::refine() {
    n.reserve(v.size());
    // vector<unique_ptr<Object> > newFace;
    vector<pair<vec3, float> > FNS(f.size());           /* Face normal and area. */
    vector<vector<int> > vNeighborFace(v.size());       /* Neighboring face for each vertex. */
    for (int i = 0; i < f.size(); ++i) {
        PlaneTriangle *face = (PlaneTriangle *)f[i].get();        /* This is not good... */
        vNeighborFace[face->id1].push_back(i);
        vNeighborFace[face->id2].push_back(i);
        vNeighborFace[face->id3].push_back(i);

        // Calculate the area and normal for each face.
        vec3 e1 = v[face->id2] - v[face->id1];
        vec3 e2 = v[face->id3] - v[face->id1];
        vec3 fN = cross(e1, e2);
        float S = length(fN);
        fN = normalize(fN);
        FNS[i] = make_pair(fN, S);

        f[i] = make_unique<NormalTriangle>(face->id1, face->id2, face->id3, v, n);
    }

    // For each vertes calculate the normal weighted by the area of the neighboring face.
    for (int i = 0; i < v.size(); ++i) {
        vec3 nV(0.0f);
        float totalS = 0.0f;
        for (const auto &fIdx : vNeighborFace[i]) {
            nV += (FNS[fIdx].first * FNS[fIdx].second);
            totalS += FNS[fIdx].second;
        }
        nV /= totalS;
        n.push_back(normalize(nV));
    }
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