#include "Sampler.h"

default_random_engine Sampler::generator;

pair<float, float> Sampler::uniformSampleCircle() {

    uniform_real<float> distribution;
    float theta = 2.0f * PI * distribution(generator);
    float radius = sqrtf(distribution(generator));
    return make_pair(radius, theta);

}