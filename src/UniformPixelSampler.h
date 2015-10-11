#ifndef UNIFORM_PIXEL_SAMPLER_HEADER
#define UNIFORM_PIXEL_SAMPLER_HEADER

#include "Sampler.h"

class UniformPixelSampler : public PixelSampler {
public:

	UniformPixelSampler(int n) : PixelSampler() {
		num = n;
		step = 1.0f / float(num);
	}

	virtual ~UniformPixelSampler() {}

	virtual void sample(int x, int y, vector<float> &samples);

private:

	int num;
	float step;
};

#endif
