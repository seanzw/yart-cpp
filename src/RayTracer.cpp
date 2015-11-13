#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>

#include "RayTracer.h"

#include "Transform.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "Sphere.h"
#include "OCTree.h"
#include "MultipleImportanceIntegrator.h"
#include "BidirectioalPathIntegrator.h"
#include "UniformPixelSampler.h"
#include "JitteredPixelSampler.h"

using namespace glm;

RayTracer::RayTracer(const string &fn) {
    scene = make_shared<Scene>();

    // Set some default value
    width = 80;
    height = 60;
    outfn = "raytracer.png";

	transforms.push(mat4(1.0f));

	/*m.ambient = vec3(0.2f, 0.2f, 0.2f);
	m.shininess = 20.0f;*/

	attenuation = vec3(1.0f, 0.0f, 0.0f);
}

void RayTracer::showProgress(int total) {

    static clock_t start = clock();
    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;

    cout << '\r';
    float percent = float(total) / float(width * height);

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
    vector<int> progress(threadnum);
    for (int i = 0; i < threadnum; ++i) {
        progress[i] = 0;
        threads[i] = new thread(&RayTracer::generate_one_thread, this, i, threadnum, &progress[i]);
    }

    while (true) {
        this_thread::sleep_for(2s);
        int total = 0;
        for (const auto &i : progress) {
            total += i;
        }
        showProgress(total);
        if (total >= width * height) {
            break;
        }
    }

    // Wait for the thread to return.
    for (int i = 0; i < threadnum; ++i) {
        threads[i]->join();
        delete threads[i];
        threads[i] = NULL;
        DEBUG(" DONE!\n");
    }

    double duration = (clock() - start) / CLOCKS_PER_SEC;
    cout << "TOTAL TIME: " << duration << endl;

    delete[] threads;
}

void RayTracer::generate_one_thread(int row_init, int row_step, int *total) {

    DEBUG("START Thread %d\n", row_init);
    vector<float> samples;
    shared_ptr<Integrator> integrator = this->integrator->copy();
    integrator->prepare(scene);
    bool debug = false;
    for (int row = row_init; row < height; row += row_step) {
        for (int col = 0; col < width; ++col) {

			// Generate samples for each pixel.
			pixelSampler->sample(row, col, samples);

			vec3 color(0.0f);
			for (auto iter = samples.begin(); iter != samples.end(); iter += 2) {
				Ray ray = this->camera->genRay(*iter, *(iter + 1));
				color += clamp(integrator->income(ray, scene), 0.0f, 1.0f);
			}

			color /= float(samples.size() / 2);
            //color = clamp(color, 0.0f, 1.0f);
            film->expose(color, row, col);

            *total += 1;
        }
    }

}

void RayTracer::rinse() {
    film->rinse(outfn);
}

RayTracer::~RayTracer() {

}

/******************************************************************************************************
 *
 * API for yyac parser.
 *
 ******************************************************************************************************/
void RayTracer::yartSize(int width, int height) {
	if (film != NULL) {
		cerr << "ERROR: Can't have more than one size." << endl;
		exit(1);
	}
	this->width = width;
	this->height = height;
	this->film = shared_ptr<Film>(new Film(width, height));
}

void RayTracer::yartIntegrator(const string &type, const vector<float> *params) {
	if (type == "MultipleImportance") {
		integrator = make_shared<MultipleImportanceIntegrator>(int((*params)[0]), int((*params)[1]), int((*params)[2]));
	}
    else if (type == "BidirectionalPath") {
        integrator = make_shared<BidirectionalPathIntegrator>(int((*params)[0]), int((*params)[1]));
    }
	else {
		cerr << "ERROR: Unsupported integrator: " << type << endl;
		exit(1);
	}
}

void RayTracer::yartPixelSampler(const string &type, int num) {
	if (type == "UniformPixelSampler") {
		pixelSampler = shared_ptr<PixelSampler>(new UniformPixelSampler(num));
	}
    else if (type == "JitteredPixelSampler") {
        pixelSampler = make_shared<JitteredPixelSampler>(num);
    }
	else {
		cerr << "ERROR: Unsupported pixel sampler: " << type << endl;
		exit(1);
	}
}

void RayTracer::yartOutput(const string &s) {
	outfn = s;
}

void RayTracer::yartObjBegin(const string &type) {
	scene->objBegin(type);
}

void RayTracer::yartObjEnd() {
	scene->objEnd();
}

void RayTracer::yartCamera(const vec3 &eye,
	const vec3 &center,
	const vec3 &up,
	float fov_degree) {
	if (this->camera != NULL) {
		cerr << "ERROR: Can't have more than one camera." << endl;
		exit(1);
	}
	this->camera = shared_ptr<Camera>(new Camera(eye, center, up, radians(fov_degree), width, height));
}

void RayTracer::yartVertex(const vec3 &v) {
	vec3 v_transformed = applyMatrix(this->transforms.top(), v);
	scene->objVertex(v_transformed);
	// this->vbuffer.push_back(v);
}

void RayTracer::yartTri(int id1, int id2, int id3) {
	scene->objTri(id1, id2, id3);
}

void RayTracer::yartSphere(const vec3 &center, float r) {
	scene->objSphere(center, r, transforms.top(), materials[materials.size() - 1]);
}

void RayTracer::yartRefineMesh() {
    scene->objRefineMesh();
}

void RayTracer::yartTranslate(float s1, float s2, float s3) {
	mat4 translate_mat = Transform::translate(s1, s2, s3);
	transforms.top() *= translate_mat;
}

void RayTracer::yartScale(float s1, float s2, float s3) {
	mat4 scale_mat = Transform::scale(s1, s2, s3);
	transforms.top() *= scale_mat;
}

void RayTracer::yartRotate(const vec3 &axis, float angle) {
	mat3 rotate_mat = Transform::rotate(angle, axis);
	transforms.top() *= mat4(vec4(rotate_mat[0], 0.0f),
		vec4(rotate_mat[1], 0.0f),
		vec4(rotate_mat[2], 0.0f),
		vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void RayTracer::yartPushTransform() {
	transforms.push(transforms.top());
}

void RayTracer::yartPopTransform() {
	if (transforms.size() <= 1) {
		cerr << "ERROR: Transform stack is empty.\n";
		exit(1);
	}
	else {
		transforms.pop();
	}
}

void RayTracer::yartDirectional(const vec3 &direction, const vec3 &color) {
	scene->lights.push_back(shared_ptr<Light>(new DirectioalLight(color, direction)));
}

void RayTracer::yartPoint(vec3 &position, const vec3 &color) {
	position = applyMatrix(transforms.top(), position);
	scene->lights.push_back(shared_ptr<Light>(new PointLight(color, position, this->attenuation)));
}

void RayTracer::yartAreaLight(const vec3 &position, const vec3 &color, const vec3 &normal, float r) {
    scene->lights.push_back(make_shared<AreaLight>(position, color, normalize(normal), r));
}

void RayTracer::yartAttenuation(const vec3 &a) {
	this->attenuation = a;
}

void RayTracer::yartMaterial(const string &brdf, const vector<float> *params) {
    materials.emplace_back(brdf, params);
    scene->objMaterial(materials[materials.size() - 1]);
}

void RayTracer::yartBuildOCTree(int level) {
	scene->objOCTree(level);
}