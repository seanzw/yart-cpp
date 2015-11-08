#include "Sampler.h"

default_random_engine Sampler::generator;

pair<float, float> Sampler::uniformSampleCircle() {

    uniform_real<float> distribution;
    float theta = 2.0f * PI * distribution(generator);
    float radius = sqrtf(distribution(generator));
    return make_pair(radius, theta);

}

vec3 Sampler::cosinSampleHemisphere(const vec3 &normal, const vec3 &t, const vec3 &s) {

    // Sample in a circle.
    auto sample = Sampler::uniformSampleCircle();

    // Transform it into the hemisphere.
    float cosTheta = sqrtf(1.0f - sample.first * sample.first);
    vec3 direction = (t * cos(sample.second) + s * sin(sample.second)) * sample.first + normal * cosTheta;
    
    return normalize(direction);
}

vec3 Sampler::uniformSampleSphere() {
    uniform_real<float> distribution;
    float x = 2.0f * distribution(generator) - 1.0f;
    float y = 2.0f * distribution(generator) - 1.0f;
    float z = 2.0f * distribution(generator) - 1.0f;
    return normalize(vec3(x, y, z));
}

float Sampler::sample1D(float low, float high) {
    uniform_real<float> distrituion(low, high);
    return distrituion(generator);
}