#include "BidirectioalPathIntegrator.h"

void BidirectionalPathIntegrator::prepare(const shared_ptr<Scene> &scene) {

}

vec3 BidirectionalPathIntegrator::income(const Ray &r, const shared_ptr<Scene> &scene) {

    vec3 L(0.0f);

    // Clear the paths.
    eyePath.clear();
    lightPath.clear();

    // Generate eye subpath.
    // Find the first vertex.
    Intersection hit(NULL, CONST_FAR);
    if (!scene->intersect(r, hit) || dot(hit.normal, r.d) >= 0.0f) {
        // If there is no intersection, simply return.
        return L;
    }
    else {
        // Initialize the first vertex in eye subpath.
        float geometry = dot(hit.normal, -r.d) / (hit.t * hit.t);
        eyePath.emplace_back(vec3(1.0f), hit, r.d, geometry, 1.0f);
    }

    // Generate the rest of the subpath.
    for (int i = 1; i < eyePathLen; ++i) {

        // Sample a new ray.
        auto sample = eyePath[i - 1].sample();
        Ray ray = sample.first;
        float p = sample.second;

        // Find the intersection.
        if (!scene->intersect(ray, hit) || dot(hit.normal, ray.d) >= 0.0f) {
            // If there is no intersection, break out.
            break;
        }

        // Find a vertex.
        vec3 alpha = eyePath[i - 1].alpha() * eyePath[i - 1].brdf(ray.d) / p;
        float geometry = dot(hit.normal, -ray.d) * dot(eyePath[i - 1].normal(), ray.d) / (hit.t * hit.t);
        eyePath.emplace_back(alpha, hit, ray.d, geometry, p);

    }

    // Now for the light subpath.
    // First sample a ray leaving the light.
    for (const auto &light : scene->lights) {
        auto sample = light->genRay();
        Ray ray = get<0>(sample);
        vec3 normal = get<1>(sample);
        float p0 = get<2>(sample);
        float p1 = get<3>(sample);

        // Get the special point on the light.
        VertexLight vertexLight(ray.o, normal, light->Le() / p0);

        // Get the first point in the scene.
        if (scene->intersect(ray, hit) && dot(ray.d, hit.normal) < 0.0f) {
            // Initialize the first vertex in light subpath.
            if (lightPathLen > 0) {
                float geometry = dot(-ray.d, hit.normal) / (hit.t * hit.t);
                vec3 alpha = vertexLight.brdf(ray.d) * vertexLight.alpha() / p1;
                lightPath.emplace_back(alpha, hit, ray.d, geometry, p1);

                // Generate the rest of the subpath.
                for (int i = 1; i < lightPathLen; ++i) {

                    // Sample a new ray.
                    auto sample = lightPath[i - 1].sample();
                    Ray ray = sample.first;
                    float p = sample.second;

                    // Find the intersection.
                    if (!scene->intersect(ray, hit) || dot(hit.normal, ray.d) >= 0.0f) {
                        // If there is no intersection, break out.
                        break;
                    }

                    // Find a vertex.
                    vec3 alpha = lightPath[i - 1].alpha() * lightPath[i - 1].brdf(ray.d) / p;
                    float geometry = dot(hit.normal, -ray.d) * dot(lightPath[i - 1].normal(), ray.d) / (hit.t * hit.t);
                    lightPath.emplace_back(alpha, hit, ray.d, geometry, p);

                }
            }

        }

        // Finish generating the two subpaths.
        // Start to connect them.

        // First the special case where there is only vertexLight.
        for (const auto &vertex : eyePath) {

            vec3 cst = connect(scene, vertex, vertexLight);
            // Get the weight.
            float weight = /*computeWeight();*/ 1.0f;
            vec3 c = weight * vertex.alpha() * cst * vertexLight.alpha();
            L += c;
        }

        // For the rest.
        for (int iEye = 0; iEye < eyePath.size(); ++iEye) {
            for (int iLight = 0; iLight < lightPath.size(); ++iLight) {
                vec3 cst = connect(scene, eyePath[iEye], lightPath[iLight]);
                float weight = computeWeight(iEye, iLight);
                vec3 c = weight * eyePath[iEye].alpha() * cst * lightPath[iLight].alpha();
                L += c;
            }
        }
    }
    return L;
}

vec3 BidirectionalPathIntegrator::connect(const shared_ptr<Scene> &scene,
    const Vertex &v1, const Vertex &v2) const {

    // Connect them.
    vec3 o = v1.point();
    vec3 d = v2.point() - o;
    if (dot(v1.normal(), d) <= 0.0f || dot(v2.normal(), -d) <= 0.0f) {
        return vec3(0.0f);
    }
    float dSquared = clamp(dot(d, d), 1.0f, CONST_FAR);
    Ray connectRay(o, normalize(d), CONST_NEAR, sqrtf(dSquared));

    if (scene->occlude(connectRay)) {
        // If this connect ray is blocked.
        return vec3(0.0f);
    }

    // Get the contribution of the connecting edge.
    float cosTheta1 = dot(v1.normal(), connectRay.d);
    float cosTheta2 = dot(v2.normal(), -connectRay.d);
    return clamp(v1.brdf(connectRay.d) * cosTheta1 * cosTheta2 / dSquared * v2.brdf(-connectRay.d),
        0.0f, 1.0f);
}

/**
 * Compute the weight with power heuristic.
 */
float BidirectionalPathIntegrator::computeWeight(int iEye, int iLight) const {

    float denominator = 1.0f;
    float p = 1.0f;
    float g = 1.0f;
    float ratio = 1.0f;
    vec3 edge(1.0f);
    vec3 unit(1.0f);

    // First compute the possibility of generating the same path 
    // by extending the eye subpath.
    
    // Special case for the first edge.
    edge = lightPath[iLight].point() - eyePath[iEye].point();
    unit = normalize(edge);
    p = eyePath[iEye].pdf(unit);   // Possibility.
    g = dot(eyePath[iEye].normal(), unit) * dot(lightPath[iLight].normal(), -unit) / dot(edge, edge);
    ratio = (p * g) / (lightPath[iLight].geometry() * lightPath[iLight].possibility());

    denominator += ratio * ratio;

    for (int i = iLight - 1; i >= 0; --i) {
        edge = lightPath[i].point() - lightPath[i + 1].point();
        unit = normalize(edge);
        p = lightPath[i + 1].pdf(unit);
        ratio *= (p * lightPath[i + 1].geometry()) / (lightPath[i].possibility() * lightPath[i].geometry());
        denominator += ratio * ratio;
    }

    // Second compute the possibility of generating the same path
    // by extending the light subpath.

    // Special case for the first edge.
    edge = eyePath[iEye].point() - lightPath[iLight].point();
    unit = normalize(edge);
    p = lightPath[iLight].pdf(unit);   // Possibility.
    // g = dot(eyePath[iEye].normal(), unit) * dot(lightPath[iLight].normal(), -unit) / dot(edge, edge);
    // g is the same.
    ratio = (p * g) / (eyePath[iEye].geometry() * eyePath[iEye].possibility());

    denominator += ratio * ratio;

    for (int i = iEye - 1; i >= 0; --i) {
        edge = eyePath[i].point() - eyePath[i + 1].point();
        unit = normalize(edge);
        p = eyePath[i + 1].pdf(unit);
        ratio *= (p * eyePath[i + 1].geometry()) / (eyePath[i].possibility() * eyePath[i].geometry());
        denominator += ratio * ratio;
    }

    return 1.0f / denominator;
}