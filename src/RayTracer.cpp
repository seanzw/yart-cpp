#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>
#include <thread>

#include "RayTracer.h"
#include "FreeImage\FreeImage.h"
#include "glm\glm.hpp"

#include "DirectionalLight.h"
#include "Transform.h"
#include "PointLight.h"
#include "Triangle.h"
#include "Sphere.h"
#include "OCTree.h"

using namespace glm;

RayTracer::RayTracer(const string &fn) : scene(objs) {
    ifstream f(fn);

    // Set some default value
    camera = NULL;
    film = NULL;
    width = 80;
    height = 60;
    recurdepth = 5;
    useTree = false;
    outfn = "raytracer.png";

    if (f.is_open()) {

        stack<mat4> transforms;
        transforms.push(mat4(1.0f));

        // material
        Material m;
        m.ambient = vec3(0.2f, 0.2f, 0.2f);
        m.shininess = 20.0f;

        vec3 attenuation(1.0f, 0.0f, 0.0f);     // default to no attenuation

        string line, cmd;
        getline(f, line);
        while (f) {
            if ((line.find_first_not_of(" \t\r\n") != string::npos) && (line[0] != '#')) {

                stringstream s(line);
                s >> cmd;
                float values[10];

                if (cmd == "size") {
                    if (film != NULL) {
                        cerr << "ERROR: Can't have more than one size." << endl;
                        exit(1);
                    }
                    readvalues(s, 2, values);
                    width = (int)values[0];
                    height = (int)values[1];
                    film = new Film(width, height);
                    DEBUG("PARSE SIZE %d, %d\n", width, height);
                }

                else if (cmd == "maxdepth") {
                    readvalues(s, 1, values);
                    recurdepth = (int)values[0];
                    DEBUG("PARSE MAXDEPTH %d\n", recurdepth);
                }

                else if (cmd == "output") {
                    s >> outfn;
                    DEBUG("PARSE OUTPUT %s\n", outfn.c_str());
                }

                else if (cmd == "camera") {
                    if (camera != NULL) {
                        cerr << "ERROR: Can't have more than one camera." << endl;
                        exit(1);
                    }
                    readvalues(s, 10, values);
                    vec3 eye(values[0], values[1], values[2]);
                    vec3 center(values[3], values[4], values[5]);
                    vec3 up(values[6], values[7], values[8]);
                    float fovradian = radians(values[9]);
                    camera = new Camera(eye, center, up, fovradian, width, height);
                    DEBUG("PARSE CAMERA e %.2f %.2f %.2f | c %.2f %.2f %.2f | u %.2f %.2f %.2f | f %.2f\n",
                        eye[0], eye[1], eye[2],
                        center[0], center[1], center[2],
                        up[0], up[1], up[2],
                        fovradian);
                }

                else if (cmd == "maxverts") {
                    // Simply ignore it.
                }

                else if (cmd == "maxvertnorms") {
                    // Simply ignore it.
                }

                else if (cmd == "vertex") {
                    readvalues(s, 3, values);
                    vec3 v(values[0], values[1], values[2]);
                    v = applyMatrix(transforms.top(), v);
                    vbuffer.push_back(v);
                    DEBUG("PARSE VERTEX %.2f %.2f %.2f\n", v[0], v[1], v[2]);
                }

                else if (cmd == "tri") {
                    readvalues(s, 3, values);
                    vec3 v1 = vbuffer[int(values[0])];
                    vec3 v2 = vbuffer[int(values[1])];
                    vec3 v3 = vbuffer[int(values[2])];
                    v1 = applyMatrix(transforms.top(), v1);
                    v2 = applyMatrix(transforms.top(), v2);
                    v3 = applyMatrix(transforms.top(), v3);
                    objs.push_back(new Triangle(m, v1, v2, v3));
                    DEBUG("PARSE TRI %d %d %d\n",
                        int(values[0]), int(values[1]), int(values[2]));
                }

                else if (cmd == "sphere") {
                    readvalues(s, 4, values);
                    objs.push_back(new Sphere(m,
                        vec3(values[0], values[1], values[2]),
                        values[3], transforms.top()));
                    DEBUG("PARSE SPHERE c %.2f %.2f %.2f | r %.2f\n",
                        values[0], values[1], values[2], values[3]);
                }

                else if (cmd == "translate") {
                    readvalues(s, 3, values);
                    mat4 translate_mat = Transform::translate(values[0], values[1], values[2]);
                    transforms.top() *= translate_mat;
                }

                else if (cmd == "scale") {
                    readvalues(s, 3, values);
                    mat4 scale_mat = Transform::scale(values[0], values[1], values[2]);
                    transforms.top() *= scale_mat;
                }

                else if (cmd == "rotate") {
                    readvalues(s, 4, values);
                    vec3 axis(values[0], values[1], values[2]);
                    mat3 rotate_mat = Transform::rotate(values[3], axis);
                    transforms.top() *= mat4(vec4(rotate_mat[0], 0.0f),
                        vec4(rotate_mat[1], 0.0f),
                        vec4(rotate_mat[2], 0.0f),
                        vec4(0.0f, 0.0f, 0.0f, 1.0f));
                }

                else if (cmd == "pushTransform") {
                    transforms.push(transforms.top());
                    DEBUG("PARSE PUSH_TRANSFORM\n");
                }

                else if (cmd == "popTransform") {
                    if (transforms.size() <= 1) {
                        cerr << "ERROR: Transform stack is empty.\n";
                        exit(1);
                    }
                    else {
                        transforms.pop();
                    }
                }

                else if (cmd == "directional") {
                    readvalues(s, 6, values);
                    vec3 direction(values[0], values[1], values[2]);
                    vec3 color(values[3], values[4], values[5]);
                    lights.push_back(new DirectioalLight(color, direction));
                    DEBUG("PARSE DIRECTIONAL d %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
                        direction[0], direction[1], direction[2],
                        color[0], color[1], color[2]);
                }

                else if (cmd == "point") {
                    readvalues(s, 6, values);
                    vec3 position(values[0], values[1], values[2]);
                    position = applyMatrix(transforms.top(), position);
                    vec3 color(values[3], values[4], values[5]);
                    lights.push_back(new PointLight(color, position, attenuation));
                    DEBUG("PARSE DIRECTIONAL p %.2f %.2f %.2f | c %.2f %.2f %.2f\n",
                        position[0], position[1], position[2],
                        color[0], color[1], color[2]);
                }

                else if (cmd == "attenuation") {
                    readvalues(s, 3, values);
                    attenuation = vec3(values[0], values[1], values[2]);
                    DEBUG("PARSE ATTENUATION %f %f %f\n",
                        attenuation[0],
                        attenuation[1],
                        attenuation[2]);
                }

                else if (cmd == "diffuse") {
                    readvalues(s, 3, values);
                    m.diffuse = vec3(values[0], values[1], values[2]);
                    DEBUG("PARSE DIFFUSE %.2f %.2f %.2f\n",
                        m.diffuse[0], m.diffuse[1], m.diffuse[2]);
                }

                else if (cmd == "ambient") {
                    readvalues(s, 3, values);
                    m.ambient = vec3(values[0], values[1], values[2]);
                    DEBUG("PARSE AMBIENT %.2f %.2f %.2f\n",
                        m.ambient[0], m.ambient[1], m.ambient[2]);
                }

                else if (cmd == "specular") {
                    readvalues(s, 3, values);
                    m.specular = vec3(values[0], values[1], values[2]);
                    DEBUG("PARSE SPECULAR %.2f %.2f %.2f\n",
                        m.specular[0], m.specular[1], m.specular[2]);
                }

                else if (cmd == "emission") {
                    readvalues(s, 3, values);
                    m.emission = vec3(values[0], values[1], values[2]);
                    DEBUG("PARSE EMISSION %.2f %.2f %.2f\n",
                        m.emission[0], m.emission[1], m.emission[2]);
                }

                else if (cmd == "shininess") {
                    readvalues(s, 1, values);
                    m.shininess = values[0];
                    DEBUG("PARSE SHININESS %.2f\n", m.shininess);
                }

                else if (cmd == "buildOCTree") {

                    // Set the max level of OCTree.
                    readvalues(s, 1, values);
                    OCTree::MAX_LEVEL = int(values[0]) >= 1 ? int(values[0]) : 1;

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
                    DEBUG("PARSE BUIILD_OCTREE LEVEL %d\n", OCTree::MAX_LEVEL);
                }

                else {
                    cerr << "Unknown Command: " << cmd << endl;
                    exit(1);
                }
            }
            getline(f, line);
        }
    }
    else {
        cerr << "RayTracer can't open input file.";
        exit(1);
    }
}

void RayTracer::readvalues(stringstream &s, int n, float *values) {
    for (int i = 0; i < n; ++i) {
        s >> values[i];
        if (s.fail()) {
            cerr << "Failed reading value " << i << endl;
            exit(1);
        }
    }
}

void RayTracer::showProgress(int row, int col) {

    static clock_t start = clock();
    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;

    cout << '\r';
    float percent = float(row * width + col) / float(width * height);

    for (int i = 0; i < percent * 32; ++i) {
        cout << "=";
    }
    cout << "> " << fixed << setprecision(2) << percent * 100.0f << "% ";
    double remain = (percent > 0.001) ? elapsed * (1.0 - percent) / percent : 1000.0;
    cout << "Elapsed " << elapsed << " Remain Time: " << remain;
}

void RayTracer::generate(int threadnum) {

    DEBUG("START RAY TRACING\n");
    clock_t start = clock();
    
    threadnum = threadnum < 1 ? 1 : (threadnum > 8 ? 8 : threadnum);
    thread **threads = new thread*[threadnum];
    for (int i = 0; i < threadnum; ++i) {
        threads[i] = new thread(&RayTracer::generate_one_thread, this, i, threadnum);
    }

    // Wait for the thread to return.
    for (int i = 0; i < threadnum; ++i) {
        threads[i]->join();
        delete threads[i];
        threads[i] = NULL;
    }

    double duration = (clock() - start) / CLOCKS_PER_SEC;
    cout << "TOTAL TIME: " << duration << endl;

    delete[] threads;
}

void RayTracer::generate_one_thread(int row_init, int row_step) {

    DEBUG("START Thread %d\n", row_init);
    for (int row = row_init; row < height; row += row_step) {
        for (int col = 0; col < width; ++col) {
            Ray ray = camera->genRay(row, col);
            vec3 color = trace(ray, 0);
            color = clamp(color, 0.0f, 1.0f);
            film->expose(color, row, col);

            // Show progress only for one thread.
            if (row_step == 1)
                showProgress(row, col);
        }
    }
    DEBUG(" DONE!\n");

}

void RayTracer::rinse() {
    film->rinse(outfn);
}

Hit RayTracer::intersect(const Ray &r) const {
    Hit hit;
    hit.t = CONST_FAR;

    // Get the intersection.
    const vector<Object*> &scene = useTree ? trees : objs;
    for (const auto &obj : scene) {
        Hit temp = obj->intersect(r);
        if (temp.t < hit.t) {
            hit = temp;
        }
    }

    return hit;
}

vec3 RayTracer::trace(const Ray &r, int level) {

    // Initialize default value.
    vec3 ret = vec3(0.0f);

    // Find the intersect.
    Hit hit = intersect(r);

    // Is there intersection?
    if (hit.t >= CONST_FAR) {
        return ret;
    }

    // Ambient.
    ret += hit.m.ambient;

    // Emission.
    ret += hit.m.emission;

    // Diffuse and specular for each light.
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    for (const auto &light : lights) {

        // Get the shadow ray and the distance to the light.
        float distanceToLight;
        Ray shadowRay = light->genShadowRay(hit.point, &distanceToLight);

        // Use the shadow ray to find intersect.
        Hit shadowHit = intersect(shadowRay);

        // Is the light blocked£¿
        if (shadowHit.t < distanceToLight) {
            continue;
        }

        // Get the light color.
        vec3 lightColor = light->getColor(distanceToLight);

        // Calculate the diffuse term.
        diffuse += lightColor * hit.m.diffuse * max(0.0f, dot(hit.normal, shadowRay.d));
        
        // Specular.
        vec3 half = normalize(shadowRay.d - r.d);
        specular += lightColor * hit.m.specular * powf(max(0.0f, dot(hit.normal, half)), hit.m.shininess);
        
    }
    ret += diffuse;
    ret += specular;

    // Mirror reflection
    if (level < recurdepth - 1) {

        // Mirror the incoming ray.
        vec3 reflectDirection = reflect(r.d, hit.normal);
        Ray reflectRay(hit.point, reflectDirection);
        vec3 specularColor = trace(reflectRay, level + 1);
        ret += hit.m.specular * specularColor;
    }

    return ret;
}

RayTracer::~RayTracer() {
    delete camera;
    camera = NULL;
    delete film;
    film = NULL;
    for (auto &light : lights) {
        delete light;
        light = NULL;
    }
    for (auto &obj : objs) {
        delete obj;
        obj = NULL;
    }
    for (auto &tree : trees) {
        delete tree;
        tree = NULL;
    }
}