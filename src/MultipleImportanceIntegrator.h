#ifndef MULTIPLE_IMPORTANCE_INTEGRATOR_HEADER
#define MULTIPLE_IMPORTANCE_INTEGRATOR_HEADER

#include "Integrator.h"

class MultipleImportanceIntegrator : public Integrator {
public:
    MultipleImportanceIntegrator(int maxDepth, int nBSDFSamples)
    : maxDepth(maxDepth), nBSDFSamples(nBSDFSamples) {
	}
	virtual ~MultipleImportanceIntegrator() {}

	virtual vec3 income(const Ray &r, const shared_ptr<Scene> &scene) const {
		return income(r, scene, 0);
	}

private:
	const int maxDepth;
    const int nBSDFSamples;

	vec3 income(const Ray &r, const shared_ptr<Scene> &scene, int level) const;

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