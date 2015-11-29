#ifndef ADAPTIVE_PIXEL_SAMPLER_HEADER
#define ADAPTIVE_PIXEL_SAMPLER_HEADER

#include "JitteredPixelSampler.h"

class AdaptivePixelSampler : public JitteredPixelSampler {
public:
    AdaptivePixelSampler(int num, int min, int max, float threshold)
        : JitteredPixelSampler(num), min(min), max(max), threshold(threshold) {
    }

    virtual ~AdaptivePixelSampler() {}

    virtual bool done(const vec3 &prevL, const vec3 &L, size_t totalSamples) const;

private:
    int min;
    int max;
    float threshold;
};

#endif