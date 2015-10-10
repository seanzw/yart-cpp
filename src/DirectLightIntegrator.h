#ifndef DIRECTLIGHT_INTEGRATOR_HEADER
#define DIRECTLIGHT_INTEGRATOR_HEADER

#include "Integrator.h"

class DirectLightIntegrator : public Integrator {
public:
	DirectLightIntegrator(int maxDepth) {
		this->maxDepth = maxDepth;
	}
	virtual ~DirectLightIntegrator() {}

	virtual vec3 income(const Ray &r,
		const shared_ptr<Scene> &scene
		) {
		return income(r, scene, 0);
	}

private:
	int maxDepth;

	virtual vec3 income(const Ray &r,
		const shared_ptr<Scene> &scene,
		int level);
};

#endif