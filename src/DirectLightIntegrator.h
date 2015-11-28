#ifndef DIRECT_LIGHT_INTEGRATOR_HEADER
#define DIRECT_LIGHT_INTEGRATOR_HEADER

#include "Integrator.h"

class DirectLightIntegrator : public Integrator {
public:
    DirectLightIntegrator(int maxDepth, int nBSDFSamples, int nLightSamples)
    : maxDepth(maxDepth), nBSDFSamples(nBSDFSamples), nLightSamples(nLightSamples) {
	}
	virtual ~DirectLightIntegrator() {}

	virtual vec3 income(const Ray &r, const shared_ptr<Scene> &scene) {
		return income(r, scene, 0);
	}

    virtual shared_ptr<Integrator> copy() const {
        return make_shared<DirectLightIntegrator>(maxDepth, nBSDFSamples, nLightSamples);
    }

private:
	const int maxDepth;
    const int nBSDFSamples;
    const int nLightSamples;

	vec3 income(const Ray &r, const shared_ptr<Scene> &scene, int level);

    // Utilities.

    // Power 2 heuristic weight function.
    inline float powerHeuristec(float p1, int n1, float p2, int n2) const {
        float w1 = p1 * n1;
        float w2 = p2 * n2;
        w1 *= w1;
        w2 *= w2;
        return w1 / (w1 + w2);
    }
};

#endif