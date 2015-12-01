#ifndef UNIFORM_PIXEL_SAMPLER_HEADER
#define UNIFORM_PIXEL_SAMPLER_HEADER

#include "Sampler.h"

class UniformPixelSampler : public PixelSampler {
public:

	UniformPixelSampler(int n) : PixelSampler() {
		num = n;
        total = n * n;
		step = 1.0f / float(num);
	}

	virtual ~UniformPixelSampler() {}

	virtual void sample(int x, int y, vector<float> &samples);
    virtual bool done(const vec3 &prevL, const vec3 &L, size_t totalSamples) const;

private:
    size_t total;
	int num;
	float step;
};

#endif
