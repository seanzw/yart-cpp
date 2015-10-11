#include "UniformPixelSampler.h"

void UniformPixelSampler::sample(int x, int y, vector<float> &samples) {
	samples.clear();
	samples.reserve(num * num * 2);
	for (int i = 0; i < num; ++i) {
		for (int j = 0; j < num; ++j) {
			samples.push_back((float)x + ((float)i + 0.5f) * step);
			samples.push_back((float)y + ((float)j + 0.5f) * step);
		}
	}
}