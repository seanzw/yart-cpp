#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "Light.h"
#include "Object.h"
#include "Mesh.h"

class Scene {
public:
	Scene() {
		useTree = false;
		isInObject = false;
	}
	~Scene() {}

	/* API for parser. */
	void objBegin(const string &type);
	void objEnd();
	void objSphere(const vec3 &center, float r, const mat4 &transform, const Material &m);
	void objVertex(const vec3 &v);
	void objTri(int id1, int id2, int id3);
    void objRefineMesh();
    void objMaterial(const Material &m);
	void objOCTree(int level);

	// Lights.
	vector<shared_ptr<Light> > lights;

	// Objects.
	vector<shared_ptr<Object> > objs;

	// Tree.
	vector<shared_ptr<Object> > trees;

	bool useTree;

	bool intersect(const Ray &r, Intersection &hit) const {
		bool isHit = false;
		hit.t = CONST_FAR;

		// Get the intersection.
		for (const auto &obj : getObjs()) {
            Intersection temp = obj->intersect(r);
			if (temp.t < hit.t) {
				isHit = true;
				hit = temp;
			}
		}

		return isHit;
	}

    // Return true if the ray is occluded.
    bool occlude(const Ray &r) const {
        for (const auto &obj : getObjs()) {
            if (obj->occlude(r)) {
                return true;
            }
        }
        return false;
    }

private:

	enum ObjType {
		SCENE_SPHERE,
		SCENE_MESH
	};

	ObjType objType;
	bool isInObject;

	shared_ptr<Mesh> mesh;

	inline const vector<shared_ptr<Object> > &getObjs() const {
		return useTree ? trees : objs;
	}

};


#endif
