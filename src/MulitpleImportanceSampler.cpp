#include "MultipleImportanceIntegrator.h"

vec3 MultipleImportanceIntegrator::income(const Ray &r,
	const shared_ptr<Scene> &scene,
	int level) const {

	vec3 L(0.0f);

	Intersection hit(NULL, CONST_FAR);

	// Is there intersection?
	if (!scene->intersect(r, hit)) {
		return L;
	}

	// Sample for each light.
	for (const auto &light : scene->lights) {
		// Get the shadow ray and the distance to the light.
        vector<pair<Ray, float> > rayPDFs;
        light->genShadowRay(hit, rayPDFs);
        int nLightSamples = rayPDFs.size();
        vec3 Lo(0.0f);
        for (const auto &rayPDF : rayPDFs) {

            Ray shadowRay = rayPDF.first;
            float lightPDF = rayPDF.second;

            // Use the shadow ray to find intersect.
            if (scene->occlude(shadowRay)) {
                continue;
            }

            // Get the light color.
            vec3 Le = light->Le(shadowRay.tmax);
            vec3 f = hit.m->brdf->brdf(hit, r.d, shadowRay.d);
            float w = powerHeuristec(lightPDF, nLightSamples, hit.m->brdf->pdf(hit, shadowRay.d), nBSDFSamples);

            // Here the geometry term is inside the pdf.
            // Use power heuristic.
            Lo +=  f * Le * w / lightPDF;

        }
        Lo /= rayPDFs.size();
        L += Lo;
	}

	// Sample for BSDF.
	if (level < maxDepth - 1) {

        vec3 LBSDF(0.0f);
        for (int i = 0; i < nBSDFSamples; ++i) {
            while (true) {
                auto sample = hit.m->brdf->sample(hit);
                if (sample.second > 0.1f) {
                    auto Li = income(sample.first, scene, level + 1);
                    vec3 f = hit.m->brdf->brdf(hit, r.d, sample.first.d);
                    float w = powerHeuristec(sample.second, nBSDFSamples, 0, 10);
                    LBSDF += f * Li * dot(hit.normal, sample.first.d) * w / sample.second;
                    break;
                }
            }
        }
        LBSDF /= nBSDFSamples;
        L += LBSDF;

		// Mirror the incoming ray.
		/*vec3 reflectDirection = reflect(r.d, hit.normal);
		Ray reflectRay(hit.point, reflectDirection, CONST_NEAR, CONST_FAR);
		vec3 specularColor = income(reflectRay, scene, level + 1);
        vec3 f = hit.m->brdf->brdf(hit, reflectRay.d);
		L += f * dot(hit.normal, reflectRay.d) * specularColor;*/

	}

	return L;
}