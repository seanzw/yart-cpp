#ifndef RAY_TRACER_HEADER
#define RAY_TRACER_HEADER

#include <string>
#include <utility>
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Film.h"

using namespace std;

class RayTracer {
public:
    RayTracer(const string &fn);
    ~RayTracer();

    void generate(int threadnum);
    void rinse();

private:

    // HELPER FUNCTIONS.
    void readvalues(stringstream &s, int n, float *values);
    void showProgress(int row, int col);
    //vec3 applyMatrix(const mat4 &trans, const vec3 &v);
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