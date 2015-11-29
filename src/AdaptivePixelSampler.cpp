#include "AdaptivePixelSampler.h"

bool AdaptivePixelSampler::done(const vec3 &prevL, const vec3 &L, size_t totalSamples) const {
    if (totalSamples >= max) {
        return true;
    }
    if (totalSamples < min) {
        return false;
    }
    vec3 delta = abs(prevL - L);
    return delta.r < threshold &&
        delta.g < threshold &&
        delta.b < threshold;

}