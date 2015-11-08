#ifndef RAY_TRACER_HEADER
#define RAY_TRACER_HEADER

#include <string>
#include <utility>
#include <stack>
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Film.h"
#include "Scene.h"
#include "Sampler.h"
#include "Integrator.h"


using namespace std;

class RayTracer {
public:
    RayTracer(const string &fn);
    ~RayTracer();

    void generate(int threadnum);
    void rinse();

	/* API for yyac parser. */
	void yartSize(int width, int height);
	void yartIntegrator(const string &type, const vector<float> *params);
	void yartPixelSampler(const string &type, int num);
	void yartOutput(const string &s);
	void yartObjBegin(const string &s);
	void yartObjEnd();
	void yartCamera(const vec3 &eye,
		const vec3 &center,
		const vec3 &up,
		float fov_degree);
	void yartVertex(const vec3 &v);
	void yartTri(int id1, int id2, int id3);
	void yartSphere(const vec3 &center, float r);
    void yartRefineMesh();
	void yartTranslate(float s1, float s2, float s3);
	void yartScale(float s1, float s2, float s3);
	void yartRotate(const vec3 &axis, float angle);
	void yartPushTransform();
	void yartPopTransform();
	void yartDirectional(const vec3 &direction, const vec3 &color);
	void yartPoint(vec3 &position, const vec3 &color);
    void yartAreaLight(const vec3 &position, const vec3 &color, const vec3 &normal, float r);
	void yartAttenuation(const vec3 &a);
    void yartMaterial(const string &brdf, const vector<float> *params);
	void yartBuildOCTree(int level);


private:

	// Something used during the parsing.
	stack<mat4> transforms;
	// Material m;
	vec3 attenuation;

    // HELPER FUNCTIONS.
    void showProgress(int total);

    void generate_one_thread(int row_init, int row_step, int *total);

    // DATA.
    int width, height;
    string outfn;

	shared_ptr<PixelSampler> pixelSampler;
	shared_ptr<Integrator> integrator;
	shared_ptr<Scene> scene;
    shared_ptr<Camera> camera;
    vector<vec3> vbuffer;
    vector<Material> materials;
    shared_ptr<Film> film;
};

#endif