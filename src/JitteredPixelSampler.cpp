#include "JitteredPixelSampler.h"

void JitteredPixelSampler::sample(int x, int y, vector<float> &samples) {
    samples.clear();
    samples.reserve(num * num * 2);
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            samples.push_back((float)x + ((float)i + Sampler::sample1D(-0.5f, 0.5f) + 0.5f) * step);
            samples.push_back((float)y + ((float)j + Sampler::sample1D(-0.5f, 0.5f) + 0.5f) * step);
        }
    }
}