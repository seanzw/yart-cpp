#include "MultipleImportanceIntegrator.h"

vec3 MultipleImportanceIntegrator::income(const Ray &r,
	const shared_ptr<Scene> &scene,
	int level) {

	vec3 L(0.0f);

	Intersection hit(NULL, NULL, CONST_FAR);

	// Is there intersection?
	if (!scene->intersect(r, hit)) {
		return L;
	}

	// Sample for each light.
	for (const auto &light : scene->lights) {
		// Get the shadow ray and the distance to the light.
        vec3 Lo(0.0f);
        for (int i = 0; i < nLightSamples; ++i) {
            auto sample = light->genShadowRay(hit);
           
            Ray shadowRay = sample.first;
            float lightPDF = sample.second;

            // Use the shadow ray to find intersect.
            if (scene->occlude(shadowRay)) {
                continue;
            }

            // Get the light color.
            vec3 Le = light->Le();
            vec3 f = hit.m->bsdf->bsdf(hit, r.d, shadowRay.d);
            float w = powerHeuristec(lightPDF, nLightSamples, hit.m->bsdf->pdf(hit, r.d, shadowRay.d), nBSDFSamples);

            // Here the geometry term is inside the pdf.
            // Use power heuristic.
            Lo += f * Le * w / lightPDF;
        }
        Lo /= nLightSamples;
        L += Lo;
	}

	// Sample for BSDF.
	if (level < maxDepth - 1) {

        vec3 LBSDF(0.0f);
        for (int i = 0; i < nBSDFSamples; ++i) {
            while (true) {
                auto sample = hit.m->bsdf->sample(hit, r.d);
                if (sample.second > 0.1f) {
                    auto Li = income(sample.first, scene, level + 1);
                    vec3 f = hit.m->bsdf->bsdf(hit, r.d, sample.first.d);
                    //float w = powerHeuristec(sample.second, nBSDFSamples, scene->lights[0]->pdf(hit, sample.first.d), nLightSamples);
                    float w = 1.0f;
                    LBSDF += w * f * Li / sample.second;
                    break;
                }
            }
        }
        LBSDF /= nBSDFSamples;
        L += LBSDF;

	}

	return L;
}