#ifndef MONTE_CARLO_INTEGRATOR_HEADER
#define MONTE_CARLO_INTEGRATOR_HEADER

#include "Integrator.h"

class MonteCarloIntegrator : public Integrator {
public:

    MonteCarloIntegrator(int nSamples, int maxDepth) : Integrator(), n(nSamples), maxDepth(maxDepth) {}

    virtual ~MonteCarloIntegrator() {}

    virtual vec3 income(const Ray &r, const shared_ptr<Scene> &scene) const {
        return income(r, scene, 0);
    }


private:

    int n;      // Number of samples.
    int maxDepth;      // Max depth

    vec3 income(const Ray &r, const shared_ptr<Scene> &scene, int level) const;

};


#endif
