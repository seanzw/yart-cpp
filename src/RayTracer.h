#ifndef RAY_TRACER_HEADER
#define RAY_TRACER_HEADER

#include <string>
#include <utility>
#include <stack>
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Film.h"
#include "Transform.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Triangle.h"
#include "Sphere.h"
#include "OCTree.h"

#include "Scene.h"
#include "DirectLightIntegrator.h"

using namespace std;

class RayTracer {
public:
    RayTracer(const string &fn);
    ~RayTracer();

    void generate(int threadnum);
    void rinse();

	/* API for yyac parser. */
	void yartSize(int width, int height) {
		if (film != NULL) {
			cerr << "ERROR: Can't have more than one size." << endl;
			exit(1);
		}
		this->width = width;
		this->height = height;
		this->film = shared_ptr<Film>(new Film(width, height));
	}

	void yartIntegrator(const string &type, int maxDepth) {
		if (type == "DirectLight") {
			integrator = shared_ptr<Integrator>(new DirectLightIntegrator(maxDepth));
		}
	}

	void yartOutput(const string &s) {
		outfn = s;
	}

	void yartCamera(const vec3 &eye,
		const vec3 &center,
		const vec3 &up,
		float fov_degree) {
		if (this->camera != NULL) {
			cerr << "ERROR: Can't have more than one camera." << endl;
			exit(1);
		}
		this->camera = shared_ptr<Camera>(new Camera(eye, center, up, radians(fov_degree), width, height));
	}

	void yartVertex(const vec3 &v) {
		vec3 v_transformed = applyMatrix(this->transforms.top(), v);
		this->vbuffer.push_back(v_transformed);
	}

	void yartTri(int id1, int id2, int id3) {
		vec3 v1 = vbuffer[id1];
		vec3 v2 = vbuffer[id2];
		vec3 v3 = vbuffer[id3];
		v1 = applyMatrix(transforms.top(), v1);
		v2 = applyMatrix(transforms.top(), v2);
		v3 = applyMatrix(transforms.top(), v3);
		scene->objs.push_back(shared_ptr<Object>(new Triangle(m, v1, v2, v3)));
	}

	void yartSphere(const vec3 &center, float r) {
		scene->objs.push_back(shared_ptr<Object>(new Sphere(m, center, r, transforms.top())));
	}

	void yartTranslate(float s1, float s2, float s3) {
		mat4 translate_mat = Transform::translate(s1, s2, s3);
		transforms.top() *= translate_mat;
	}

	void yartScale(float s1, float s2, float s3) {
		mat4 scale_mat = Transform::scale(s1, s2, s3);
		transforms.top() *= scale_mat;
	}

	void yartRotate(const vec3 &axis, float angle) {
		mat3 rotate_mat = Transform::rotate(angle, axis);
		transforms.top() *= mat4(vec4(rotate_mat[0], 0.0f),
			vec4(rotate_mat[1], 0.0f),
			vec4(rotate_mat[2], 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void yartPushTransform() {
		transforms.push(transforms.top());
	}

	void yartPopTransform() {
		if (transforms.size() <= 1) {
			cerr << "ERROR: Transform stack is empty.\n";
			exit(1);
		}
		else {
			transforms.pop();
		}
	}

	void yartDirectional(const vec3 &direction, const vec3 &color) {
		scene->lights.push_back(shared_ptr<Light>(new DirectioalLight(color, direction)));
	}

	void yartPoint(vec3 &position, const vec3 &color) {
		position = applyMatrix(transforms.top(), position);
		scene->lights.push_back(shared_ptr<Light>(new PointLight(color, position, this->attenuation)));
	}

	void yartAttenuation(const vec3 &a) {
		this->attenuation = a;
	}

	void yartDiffuse(const vec3 &d) {
		this->m.diffuse = d;
	}

	void yartAmbient(const vec3 &a) {
		this->m.ambient = a;
	}

	void yartSpecular(const vec3 &s) {
		this->m.specular = s;
	}

	void yartEmission(const vec3 &e) {
		this->m.emission = e;
	}

	void yartShininess(float s) {
		this->m.shininess = s;
	}

	void yartBuildOCTree(int level) {
		OCTree::MAX_LEVEL = level > 1 ? level : 1;
		BBox b;
		for (const auto obj : scene->objs) {
			b = b.merge(obj->getBBox());
		}
		vector<int> idx;
		for (unsigned int i = 0; i < scene->objs.size(); ++i) {
			idx.push_back(i);
		}
		scene->trees.push_back(shared_ptr<Object>(new OCTree(scene->objs, b, idx, 0)));
		scene->useTree = true;
	}


private:

	// Something used during the parsing.
	stack<mat4> transforms;
	Material m;
	vec3 attenuation;

    // HELPER FUNCTIONS.
    void readvalues(stringstream &s, int n, float *values);
    void showProgress(int row, int col);

    void generate_one_thread(int row_init, int row_step);

    // DATA.
    int width, height;
    string outfn;

	shared_ptr<Integrator> integrator;

	shared_ptr<Scene> scene;

    shared_ptr<Camera> camera;

    vector<vec3> vbuffer;

    shared_ptr<Film> film;
};

#endif