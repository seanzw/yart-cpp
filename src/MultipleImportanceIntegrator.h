#ifndef MULTIPLE_IMPORTANCE_INTEGRATOR_HEADER
#define MULTIPLE_IMPORTANCE_INTEGRATOR_HEADER

#include "Integrator.h"

class MultipleImportanceIntegrator : public Integrator {
public:
    MultipleImportanceIntegrator(int maxDepth, int nBSDFSamples, int nLightSamples)
    : maxDepth(maxDepth), nBSDFSamples(nBSDFSamples), nLightSamples(nLightSamples) {
	}
	virtual ~MultipleImportanceIntegrator() {}

	virtual vec3 income(const Ray &r, const shared_ptr<Scene> &scene) {
		return income(r, scene, 0);
	}

    virtual shared_ptr<Integrator> copy() const {
        return make_shared<MultipleImportanceIntegrator>(maxDepth, nBSDFSamples, nLightSamples);
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