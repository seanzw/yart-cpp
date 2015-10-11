#ifndef SAMPLER_HEADER
#define SAMPLER_HEADER

#include "Const.h"

class Sampler {
public:
	Sampler() {}
	virtual ~Sampler() {}

	/*virtual void sample(int i, int j, vector<float> &samplers) = 0;*/
};

class PixelSampler : public Sampler {
public:
	PixelSampler() : Sampler() {}
	virtual ~PixelSampler() {}

	virtual void sample(int x, int y, vector<float> &samples) = 0;
};

#endif
