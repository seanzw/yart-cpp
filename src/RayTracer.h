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

using namespace std;

class RayTracer {
public:
    RayTracer(const string &fn);
    ~RayTracer();

    void generate(int threadnum);
    void rinse();

	/* API for yyac parser. */
	void yart_size(int width, int height) {
		if (film != NULL) {
			cerr << "ERROR: Can't have more than one size." << endl;
			exit(1);
		}
		this->width = width;
		this->height = height;
		this->film = new Film(width, height);
	}

	void yart_maxdepth(int depth) {
		recurdepth = depth;
	}

	void yart_output(const string &s) {
		outfn = s;
	}

	void yart_camera(const vec3 &eye,
		const vec3 &center,
		const vec3 &up,
		float fov_degree) {
		if (this->camera != NULL) {
			cerr << "ERROR: Can't have more than one camera." << endl;
			exit(1);
		}
		this->camera = new Camera(eye, center, up, radians(fov_degree), width, height);
	}

	void yart_vertex(const vec3 &v) {
		vec3 v_transformed = applyMatrix(this->transforms.top(), v);
		this->vbuffer.push_back(v_transformed);
	}

	void yart_tri(int id1, int id2, int id3) {
		vec3 v1 = vbuffer[id1];
		vec3 v2 = vbuffer[id2];
		vec3 v3 = vbuffer[id3];
		v1 = applyMatrix(transforms.top(), v1);
		v2 = applyMatrix(transforms.top(), v2);
		v3 = applyMatrix(transforms.top(), v3);
		this->objs.push_back(new Triangle(m, v1, v2, v3));
	}

	void yart_sphere(const vec3 &center, float r) {
		this->objs.push_back(new Sphere(m, center, r, transforms.top()));
	}

	void yart_translate(float s1, float s2, float s3) {
		mat4 translate_mat = Transform::translate(s1, s2, s3);
		transforms.top() *= translate_mat;
	}

	void yart_scale(float s1, float s2, float s3) {
		mat4 scale_mat = Transform::scale(s1, s2, s3);
		transforms.top() *= scale_mat;
	}

	void yart_rotate(const vec3 &axis, float angle) {
		mat3 rotate_mat = Transform::rotate(angle, axis);
		transforms.top() *= mat4(vec4(rotate_mat[0], 0.0f),
			vec4(rotate_mat[1], 0.0f),
			vec4(rotate_mat[2], 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void yart_pushTransform() {
		transforms.push(transforms.top());
	}

	void yart_popTransform() {
		if (transforms.size() <= 1) {
			cerr << "ERROR: Transform stack is empty.\n";
			exit(1);
		}
		else {
			transforms.pop();
		}
	}

	void yart_directional(const vec3 &direction, const vec3 &color) {
		lights.push_back(new DirectioalLight(color, direction));
	}

	void yart_point(vec3 &position, const vec3 &color) {
		position = applyMatrix(transforms.top(), position);
		lights.push_back(new PointLight(color, position, this->attenuation));
	}

	void yart_attenuation(const vec3 &a) {
		this->attenuation = a;
	}

	void yart_diffuse(const vec3 &d) {
		this->m.diffuse = d;
	}

	void yart_ambient(const vec3 &a) {
		this->m.ambient = a;
	}

	void yart_specular(const vec3 &s) {
		this->m.specular = s;
	}

	void yart_emission(const vec3 &e) {
		this->m.emission = e;
	}

	void yart_shininess(float s) {
		this->m.shininess = s;
	}

	void yart_buildOCTree(int level) {
		OCTree::MAX_LEVEL = level > 1 ? level : 1;
		BBox b;
		for (const auto obj : objs) {
			b = b.merge(obj->getBBox());
		}
		vector<int> idx;
		for (unsigned int i = 0; i < objs.size(); ++i) {
			idx.push_back(i);
		}
		trees.push_back(new OCTree(objs, b, idx, 0));
		useTree = true;
	}


private:

	// Something used during the parsing.
	stack<mat4> transforms;
	Material m;
	vec3 attenuation;

    // HELPER FUNCTIONS.
    void readvalues(stringstream &s, int n, float *values);
    void showProgress(int row, int col);
    Hit intersect(const Ray &r) const;

    void generate_one_thread(int row_init, int row_step);
    vec3 trace(const Ray &r, int level);

    // DATA.
    int width, height;
    int recurdepth;
    string outfn;
    bool useTree;

    Camera *camera;
    vector<Light*> lights;

    vector<vec3> vbuffer;
    //vector<pair<vec3, vec3> > vnbuffer;

    vector<Object*> objs;
    vector<Object *> trees;

    vector<Object *> &scene;

    Film *film;
};

#endif