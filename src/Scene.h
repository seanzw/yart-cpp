#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "Light.h"
#include "Object.h"
#include "OCTree.h"

class Scene {
public:
	Scene() {
		useTree = false;
	}
	~Scene() {}

	// Lights.
	vector<shared_ptr<Light> > lights;

	// Objects.
	vector<shared_ptr<Object> > objs;

	// Tree.
	vector<shared_ptr<Object> > trees;

	bool useTree;

	bool intersect(const Ray &r, Hit &hit) const {
		bool isHit = false;
		hit.t = CONST_FAR;

		// Get the intersection.
		for (const auto &obj : getObjs()) {
			Hit temp = obj->intersect(r);
			if (temp.t < hit.t) {
				isHit = true;
				hit = temp;
			}
		}

		return isHit;
	}

private:

	inline const vector<shared_ptr<Object> > &getObjs() const {
		return useTree ? trees : objs;
	}

};


#endif
