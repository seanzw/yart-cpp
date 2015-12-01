#ifndef JITTERED_PIXEL_SAMPLER_HEADER
#define JITTERED_PIXEL_SAMPLER_HEADER

#include "Sampler.h"
class JitteredPixelSampler : public PixelSampler {
public:
    JitteredPixelSampler(int n) : PixelSampler() {
        num = n;
        total = n * n;
        step = 1.0f / float(num);
    }

    virtual ~JitteredPixelSampler() {}

    virtual void sample(int x, int y, vector<float> &samples);
    virtual bool done(const vec3 &prevL, const vec3 &L, size_t totalSamples) const;

protected:
    size_t total;
    int num;
    float step;

};

#endif