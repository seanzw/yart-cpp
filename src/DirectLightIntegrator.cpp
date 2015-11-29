#include "DirectLightIntegrator.h"

vec3 DirectLightIntegrator::income(const Ray &r,
	const shared_ptr<Scene> &scene,
	int level) {

	vec3 L(0.0f);

	Intersection hit(NULL, NULL, CONST_FAR);

	// Is there intersection?
	if (!scene->intersect(r, hit)) {
		return L;
	}

    // Is this intersection on the light?
    if (hit.type == INTERSECTION_LIGHT) {
        // Simply return the color from the light.
        const Light *light = static_cast<const Light *>(hit.intersectable);
        return light->Le() * clamp(dot(hit.normal, -r.d), 0.0f, 1.0f);
    }

	// Sample for one light.
    if (!hit.m->bsdf->isDelta()) {

        // This is not a delta brdf. Randomly pick one light.
        int lightId = floor(Sampler::sample1D(0.0f, (float)scene->lights.size() - 0.1f));
        const shared_ptr<Light> &light = scene->lights[lightId];

        // Get the shadow ray and the distance to the light.
        vec3 Lo(0.0f);
        for (int i = 0; i < nLightSamples; ++i) {
            auto sample = light->genShadowRay(hit);

            Ray shadowRay = sample.first;
            float lightPDF = sample.second;

            if (lightPDF < CONST_EPSILON) {
                continue;
            }

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

        // Sample for BSDF.
        if (level < maxDepth - 1) {

            vec3 LBSDF(0.0f);
            for (int i = 0; i < nBSDFSamples; ++i) {
                while (true) {
                    auto sample = hit.m->bsdf->sample(hit, r.d);
                    if (sample.second > 0.1f) {
                        auto Li = income(sample.first, scene, level + 1);
                        vec3 f = hit.m->bsdf->bsdf(hit, r.d, sample.first.d);

                        float w = powerHeuristec(sample.second, nBSDFSamples, light->pdfRay(hit, sample.first.d), nLightSamples);
                        //float w = 1.0f;
                        LBSDF += w * f * Li / sample.second;
                        break;
                    }
                }
            }
            LBSDF /= nBSDFSamples;
            L += clamp(LBSDF, 0.0f, 1.0f);

        }
    }
    else {
        // This is a delta brdf. Just sample the delta direction.
        if (level < maxDepth - 1) {
            auto sample = hit.m->bsdf->sample(hit, r.d);
            auto Li = income(sample.first, scene, level + 1);
            vec3 f = hit.m->bsdf->bsdf(hit, r.d, sample.first.d);
            L += f * Li;
        }
    }

	return L;
}