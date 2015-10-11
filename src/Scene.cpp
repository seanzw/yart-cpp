#include "Scene.h"
#include "Sphere.h"

void Scene::objBegin(const string &type) {
	if (type == "Sphere") {
		objType = SCENE_SPHERE;
		isInObject = true;
	}
	else if (type == "Mesh") {
		objType = SCENE_MESH;
		mesh = shared_ptr<Mesh>(new Mesh);
		isInObject = true;
	}
	else {		
		exit(1);
	}
}

void Scene::objEnd() {
	if (isInObject && objType == SCENE_MESH) {
		objs.push_back(mesh);
	}
	isInObject = false;
}

void Scene::objSphere(const vec3 &center, float r, const mat4 &transform, const Material &m) {
	if (isInObject && objType == SCENE_SPHERE)
		objs.push_back(shared_ptr<Object>(new Sphere(m, center, r, transform)));
	else {
		cerr << "ERROR: CAN NOT CREATE SPHERE. " << endl;
	}
}

void Scene::objVertex(const vec3 &v) {
	if (isInObject && objType == SCENE_MESH) {
		mesh->pushVertex(v);
	}
}

void Scene::objTri(const Material &m, int id1, int id2, int id3) {
	if (isInObject && objType == SCENE_MESH) {
		mesh->pushTri(m, id1, id2, id3);
	}
}

void Scene::objOCTree(int level) {
	if (isInObject && objType == SCENE_MESH) {
		mesh->buildOCTree(level);
	}
}