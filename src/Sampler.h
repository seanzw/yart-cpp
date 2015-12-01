#ifndef SAMPLER_HEADER
#define SAMPLER_HEADER

#include <random>
#include "Const.h"

using std::default_random_engine;

class Sampler {
public:
	Sampler() {}
	virtual ~Sampler() {}

	/*virtual void sample(int i, int j, vector<float> &samplers) = 0;*/

    /* Some static utility function. */
    static pair<float, float> uniformSampleCircle();
    static vec3 cosinSampleHemisphere(const vec3 &normal, const vec3 &tangent, const vec3 &cotangent);
    static vec3 uniformSampleSphere();
    static float sample1D(float low, float high);

protected:
    static default_random_engine generator;
};

class PixelSampler : public Sampler {
public:
	PixelSampler() : Sampler() {}
	virtual ~PixelSampler() {}

	virtual void sample(int x, int y, vector<float> &samples) = 0;
    virtual bool done(const vec3 &prevL, const vec3 &L, size_t totalSamples) const = 0;
};

#endif
