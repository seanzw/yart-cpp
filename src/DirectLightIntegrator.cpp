#include "DirectLightIntegrator.h"

vec3 DirectLightIntegrator::income(const Ray &r,
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
        for (const auto &rayPDF : rayPDFs) {

            Ray shadowRay = rayPDF.first;
            float lightPDF = rayPDF.second;

            // Use the shadow ray to find intersect.
            if (scene->occlude(shadowRay)) {
                continue;
            }

            // Get the light color.
            vec3 Le = light->Le(shadowRay.tmax);
            vec3 f = hit.m->brdf->brdf(hit, shadowRay.d);
            L +=  f * dot(hit.normal, shadowRay.d) * Le / lightPDF;

        }
        L /= rayPDFs.size();
	}

	// Mirror reflection
	if (level < maxDepth - 1) {

		// Mirror the incoming ray.
		vec3 reflectDirection = reflect(r.d, hit.normal);
		Ray reflectRay(hit.point, reflectDirection, CONST_NEAR, CONST_FAR);
		vec3 specularColor = income(reflectRay, scene, level + 1);
        vec3 f = hit.m->brdf->brdf(hit, reflectRay.d);
		L += f * dot(hit.normal, reflectRay.d) * specularColor;

	}

	return L;
}