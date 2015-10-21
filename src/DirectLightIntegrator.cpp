#include "DirectLightIntegrator.h"

vec3 DirectLightIntegrator::income(const Ray &r,
	const shared_ptr<Scene> &scene,
	int level) const {

	vec3 L(0.0f);

	Hit hit;

	// Is there intersection?
	if (!scene->intersect(r, hit)) {
		return L;
	}

    // Total Samples we take.
    size_t nSamples = 0;

	L += hit.m.ambient;

	// Emission.
	L += hit.m.emission;

	// Sample for each light.
	for (const auto &light : scene->lights) {
		// Get the shadow ray and the distance to the light.
        vector<pair<Ray, float> > rayPDFs;
        light->genShadowRay(hit, rayPDFs);
        nSamples += rayPDFs.size();
        for (const auto &rayPDF : rayPDFs) {

            Ray shadowRay = rayPDF.first;
            float lightPDF = rayPDF.second;

            // Use the shadow ray to find intersect.
            Hit shadowHit;
            if (scene->occlude(shadowRay)) {
                continue;
            }

            // Get the light color.
            vec3 lightColor = light->Le(shadowRay.tmax);

            // Calculate the diffuse term.
            L += lightColor * hit.m.diffuse * max(0.0f, dot(hit.normal, shadowRay.d)) / lightPDF;

            // Specular.
            vec3 half = normalize(shadowRay.d - r.d);
            L += lightColor * hit.m.specular * powf(max(0.0f, dot(hit.normal, half)), hit.m.shininess) / lightPDF;
        }
	}

	// Mirror reflection
	if (level < maxDepth - 1) {

		// Mirror the incoming ray.
		vec3 reflectDirection = reflect(r.d, hit.normal);
		Ray reflectRay(hit.point, reflectDirection, CONST_NEAR, CONST_FAR);
		vec3 specularColor = income(reflectRay, scene, level + 1);
		L += hit.m.specular * specularColor;
	}

	return L / float(nSamples);
}