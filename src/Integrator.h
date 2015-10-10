#ifndef INTEGRATOR_HEADER
#define INTEGRATOR_HEADER

#include "Scene.h"

class Integrator {
public:
	Integrator() {}
	virtual ~Integrator() {}

	virtual vec3 income(const Ray &r,
		const shared_ptr<Scene> &scene
		) = 0;

};

#endif