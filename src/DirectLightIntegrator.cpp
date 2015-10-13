#include "DirectLightIntegrator.h"

vec3 DirectLightIntegrator::income(const Ray &r,
	const shared_ptr<Scene> &scene,
	int level) {

	vec3 L(0.0f);

	Hit hit;

	// Is there intersection?
	if (!scene->intersect(r, hit)) {
		return L;
	}

	L += hit.m.ambient;

	// Emission.
	L += hit.m.emission;

	// Sample for each light.
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	for (const auto &light : scene->lights) {
		// Get the shadow ray and the distance to the light.
        auto rays = make_unique<vector<Ray> >();
        light->genShadowRay(hit.point, rays);
        for (const auto &shadowRay : *rays) {

            // Use the shadow ray to find intersect.
            Hit shadowHit;
            if (scene->occlude(shadowRay)) {
                continue;
            }

            // Get the light color.
            vec3 lightColor = light->getColor(shadowRay.tmax);

            // Calculate the diffuse term.
            diffuse += lightColor * hit.m.diffuse * max(0.0f, dot(hit.normal, shadowRay.d));

            // Specular.
            vec3 half = normalize(shadowRay.d - r.d);
            specular += lightColor * hit.m.specular * powf(max(0.0f, dot(hit.normal, half)), hit.m.shininess);
        }
        diffuse /= rays->size();
        specular /= rays->size();

	}
	L += diffuse;
	L += specular;

	// Mirror reflection
	if (level < maxDepth - 1) {

		// Mirror the incoming ray.
		vec3 reflectDirection = reflect(r.d, hit.normal);
		Ray reflectRay(hit.point, reflectDirection, CONST_NEAR, CONST_FAR);
		vec3 specularColor = income(reflectRay, scene, level + 1);
		L += hit.m.specular * specularColor;
	}

	return L;
}