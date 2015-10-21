#ifndef JITTERED_PIXEL_SAMPLER_HEADER
#define JITTERED_PIXEL_SAMPLER_HEADER

#include "Sampler.h"
class JitteredPixelSampler : public PixelSampler {
public:
    JitteredPixelSampler(int n) : PixelSampler() {
        num = n;
        step = 1.0f / float(num);
    }

    virtual ~JitteredPixelSampler() {}

    virtual void sample(int x, int y, vector<float> &samples);

private:

    int num;
    float step;
};

#endif