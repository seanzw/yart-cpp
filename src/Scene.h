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

	inline vector<shared_ptr<Object> > &getObjs() {
		return useTree ? trees : objs;
	}

	bool useTree;

private:

};


#endif
