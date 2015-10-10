#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>

#include "RayTracer.h"


using namespace glm;

RayTracer::RayTracer(const string &fn) {
	scene = shared_ptr<Scene>(new Scene());

    // Set some default value
    width = 80;
    height = 60;
    outfn = "raytracer.png";

	transforms.push(mat4(1.0f));

	m.ambient = vec3(0.2f, 0.2f, 0.2f);
	m.shininess = 20.0f;

	attenuation = vec3(1.0f, 0.0f, 0.0f);
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
            Ray ray = this->camera->genRay(row, col);
			vec3 color = integrator->income(ray, scene);
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

RayTracer::~RayTracer() {

}