#ifndef INTEGRATOR_HEADER
#define INTEGRATOR_HEADER

#include "Scene.h"

class Integrator {
public:
	Integrator() {}
	virtual ~Integrator() {}

    // Any pre-computation?
    virtual void prepare(const shared_ptr<Scene> &scene) {

    }

    // Return a copy of the integrator.
    // Used for multiple threads.
    virtual shared_ptr<Integrator> copy() const = 0;

	virtual vec3 income(const Ray &r,
		const shared_ptr<Scene> &scene
		) = 0;

};

#endif