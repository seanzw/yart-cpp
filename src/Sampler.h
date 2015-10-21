#ifndef SAMPLER_HEADER
#define SAMPLER_HEADER

#include <random>
#include "Const.h"

class Sampler {
public:
	Sampler() {}
	virtual ~Sampler() {}

	/*virtual void sample(int i, int j, vector<float> &samplers) = 0;*/

    /* Some static utility function. */
    static pair<float, float> uniformSampleCircle();
    static float sample1D(float low, float high);

protected:
    static default_random_engine generator;
};

class PixelSampler : public Sampler {
public:
	PixelSampler() : Sampler() {}
	virtual ~PixelSampler() {}

	virtual void sample(int x, int y, vector<float> &samples) = 0;
};

#endif
