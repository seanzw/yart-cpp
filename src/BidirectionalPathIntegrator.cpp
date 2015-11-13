#include "BidirectioalPathIntegrator.h"

void BidirectionalPathIntegrator::prepare(const shared_ptr<Scene> &scene) {

}

vec3 BidirectionalPathIntegrator::income(const Ray &r, const shared_ptr<Scene> &scene) {

    vec3 L(0.0f);

    // Clear the paths and buffers.
    eyePath.clear();
    eyePathBuf.clear();

    // Generate eye subpath.
    // Find the first vertex.
    Intersection hit(NULL, NULL, CONST_FAR);
    if (!scene->intersect(r, hit) || dot(hit.normal, r.d) >= 0.0f) {
        // If there is no intersection, simply return.
        return L;
    }
    else if (hit.type == INTERSECTION_LIGHT) {
        // If this is an intersection with the light.
        // simply return the color from the light.
        const Light *light = static_cast<const Light *>(hit.intersectable);
        return light->Le() * dot(hit.normal, -r.d);
    }
    else {
        // Initialize the first vertex in eye subpath.
        float geometry = dot(hit.normal, -r.d) / (hit.t * hit.t);
        pushVertexObj(eyePath, eyePathBuf, vec3(1.0f), hit, r.d, 1.0f, 1.0f, hit.m->brdf->isDelta());
    }

    // Generate the rest of the subpath.
    for (int i = 1; i < eyePathLen; ++i) {

        // Sample a new ray.
        auto sample = eyePath[i - 1]->sample();
        Ray ray = sample.first;
        float probFWD = sample.second;

        // Find the intersection.
        if (!scene->intersect(ray, hit) || dot(hit.normal, ray.d) >= 0.0f) {
            // If there is no intersection, break out.
            break;
        }
        else if (hit.type == INTERSECTION_LIGHT) {
            // If we intersect with a light, take this as a VertexLight
            // and connect it to the eye subpath.
            // Then break out to terminate the eye subpath.

            // Construct the vertexLight.
            const Light *light = static_cast<const Light *>(hit.intersectable);
            VertexLight vertexLight(hit.point, hit.normal, light->Le(), light->pdfPoint(hit.point));
            L += computeRadianceZeroLightVertex(vertexLight, ray.d, hit.t, probFWD);

            break;
        }
        
        // Find a vertex on the object.
        vec3 alpha = eyePath[i - 1]->alpha * eyePath[i - 1]->brdf(ray.d) / probFWD;
        float geometry = dot(hit.normal, -ray.d) * dot(eyePath[i - 1]->normal, ray.d) / (hit.t * hit.t);
        pushVertexObj(eyePath, eyePathBuf, alpha, hit, ray.d, geometry, probFWD, hit.m->brdf->isDelta());

    }

    // Now for the light subpath.
    // First sample a point on the light.
    for (const auto &light : scene->lights) {

        lightPath.clear();
        lightPathBuf.clear();

        auto sample = light->samplePoint();
        vec3 point = get<0>(sample);
        vec3 normal = get<1>(sample);
        float p = get<2>(sample);

        // Get the special point on the light.
        VertexLight vertexLight(point, normal, light->Le() / p, p);
        lightPath.emplace_back(&vertexLight);

        for (int i = 0; i < lightPathLen; ++i) {

            // Sample a new ray.
            auto sample = lightPath[i]->sample();
            Ray ray = sample.first;
            float p = sample.second;

            // Find the intersection.
            if (!scene->intersect(ray, hit) ||
                dot(hit.normal, ray.d) >= 0.0f ||
                hit.type == INTERSECTION_LIGHT) {
                // If there is no intersection, or we intersect a light,
                // break out.
                break;
            }

            // Find a vertex.
            vec3 alpha = lightPath[i]->alpha * lightPath[i]->brdf(ray.d) / p;
            float geometry = dot(hit.normal, -ray.d) * dot(lightPath[i]->normal, ray.d) / (hit.t * hit.t);
            pushVertexObj(lightPath, lightPathBuf, alpha, hit, ray.d, geometry, p, hit.m->brdf->isDelta());
        }


        // Finish generating the two subpaths.
        // Start to connect them.
        L += computeRadianceGeneral(scene);
    }
    return L;
}

vec3 BidirectionalPathIntegrator::connect(const shared_ptr<Scene> &scene,
    const Vertex *v1, const Vertex *v2) const {

    // Connect them.
    vec3 o = v1->point;
    vec3 d = v2->point - o;
    if (dot(v1->normal, d) <= 0.0f || dot(v2->normal, -d) <= 0.0f) {
        return vec3(0.0f);
    }
    float dSquared = dot(d, d);
    Ray connectRay(o, normalize(d), CONST_NEAR, sqrtf(dSquared));

    if (scene->occlude(connectRay)) {
        // If this connect ray is blocked.
        return vec3(0.0f);
    }

    // Get the contribution of the connecting edge.
    float cosTheta1 = dot(v1->normal, connectRay.d);
    float cosTheta2 = dot(v2->normal, -connectRay.d);
    return v1->brdf(connectRay.d) * cosTheta1 * cosTheta2 / dSquared * v2->brdf(-connectRay.d);
}

/**
 * Compute the radiance for zero light vertex.
 */
vec3 BidirectionalPathIntegrator::computeRadianceZeroLightVertex(const VertexLight &vertexLight,
    const vec3 &toLight, float t, float probFWD) const {
    const Vertex *last = eyePath[eyePath.size() - 1];
    float g = dot(last->normal, toLight) * dot(vertexLight.normal, -toLight) / (t * t);
    float weight = computeWeightZeroLightVertex(vertexLight, toLight, t, probFWD, g);
    return weight * last->alpha * last->brdf(toLight) * vertexLight.alpha / probFWD;
}

/**
 * Compute the weight for zero light vertex.
 */
float BidirectionalPathIntegrator::computeWeightZeroLightVertex(const VertexLight &vertexLight,
    const vec3 &toLight,
    float t,
    float probFWD,
    float g
    ) const {
    float denominator = 1.0f;
    float p = 1.0f;
    float ratio = 1.0f;

    // First  calculat the g.
    const Vertex *last = eyePath[eyePath.size() - 1];

    // Special case fot one light vertex.
    ratio = vertexLight.probPrev / (probFWD * g);
    if (!last->isDelta) {
        denominator += ratio * ratio;
    }

    // Special case for two light vertex.
    p = vertexLight.pdf(-toLight);
    ratio *= (p * g) / (last->probPrev * last->geometry);
    if (eyePath.size() == 1 || (!last->isDelta && !eyePath[eyePath.size() - 2]->isDelta)) {
        denominator += ratio * ratio;
    }

    // General case.
    for (int i = eyePath.size() - 2; i >= 0; --i) {
        ratio *= (eyePath[i + 1]->probBWD * eyePath[i + 1]->geometry) /
            (eyePath[i]->probPrev * eyePath[i]->geometry);
        if (i == 0 || (!eyePath[i]->isDelta && !eyePath[i - 1]->isDelta)) {
            denominator += ratio * ratio;
        }
    }

    return 1.0f / denominator;
}

/**
 * Compute the radiance in general case.
 */
vec3 BidirectionalPathIntegrator::computeRadianceGeneral(const shared_ptr<Scene> &scene) const {
    vec3 radiance(0.0f);
    for (int iEye = 0; iEye < eyePath.size(); ++iEye) {
        for (int iLight = 0; iLight < lightPath.size(); ++iLight) {
            vec3 cst = connect(scene, eyePath[iEye], lightPath[iLight]);
            float weight = computeWeightGeneral(iEye, iLight);
            radiance += weight * eyePath[iEye]->alpha * cst * lightPath[iLight]->alpha;
        }
    }
    return radiance;
}

/**
 * Compute the weight with power heuristic.
 */
float BidirectionalPathIntegrator::computeWeightGeneral(int iEye, int iLight) const {

    float denominator = 1.0f;
    float p = 1.0f;
    float ratio = 1.0f;

    // First update eyePath[iEye] and lightPath[iLight] backward probability by the connecting edge.
    vec3 edge = lightPath[iLight]->point - eyePath[iEye]->point;
    vec3 edgeUnit = normalize(edge);
    float iEyeProbBWD = eyePath[iEye]->pdfInv(edgeUnit);
    float iLightProbBWD = lightPath[iLight]->pdfInv(-edgeUnit);
    float g = dot(eyePath[iEye]->normal, edgeUnit) * dot(lightPath[iLight]->normal, -edgeUnit) / dot(edge, edge);

    /*************************************************************************************
     * Extend the eye subpath.
     *************************************************************************************/

    // Special case for the first edge.
    p = eyePath[iEye]->pdf(edgeUnit);
    ratio = (p * g) / (lightPath[iLight]->geometry * lightPath[iLight]->probPrev);
    if (iLight == 0 || !lightPath[iLight - 1]->isDelta) {
        denominator += ratio * ratio;
    }

    // Special case for the second edge.
    if (iLight > 0) {
        ratio *= (iLightProbBWD * lightPath[iLight]->geometry) /
            (lightPath[iLight - 1]->probPrev * lightPath[iLight - 1]->geometry);
        if (iLight == 1 || !lightPath[iLight - 2]->isDelta) {
            denominator += ratio * ratio;
        }
    }

    // General case.
    for (int i = iLight - 2; i >= 0; --i) {
        ratio *= (lightPath[i + 1]->probBWD * lightPath[i + 1]->geometry) /
            (lightPath[i]->probPrev * lightPath[i]->geometry);
        if (i == 0 || (!lightPath[i]->isDelta && !lightPath[i - 1]->isDelta)) {
            denominator += ratio * ratio;
        }
    }

    /*************************************************************************************
     * Extend the light subpath.
     *************************************************************************************/

    // Special case for the first edge.
    p = lightPath[iLight]->pdf(-edgeUnit);
    ratio = (p * g) / (eyePath[iEye]->geometry * eyePath[iEye]->probPrev);
    if (iEye == 0 || !eyePath[iEye - 1]->isDelta) {
        denominator += ratio * ratio;
    }

    // Special case for the second edge.
    if (iEye > 0) {
        ratio *= (iEyeProbBWD * eyePath[iEye]->geometry) /
            (eyePath[iEye - 1]->probPrev * eyePath[iEye - 1]->geometry);
        if (iEye == 1 || !eyePath[iEye - 2]->isDelta) {
            denominator += ratio * ratio;
        }
    }

    // General case.
    for (int i = iEye - 2; i >= 0; --i) {
        ratio *= (eyePath[i + 1]->probBWD * eyePath[i + 1]->geometry) /
            (eyePath[i]->probPrev * eyePath[i]->geometry);
        if (i == 0 || (!eyePath[i]->isDelta && !eyePath[i - 1]->isDelta)) {
            denominator += ratio * ratio;
        }
    }

    return 1.0f / denominator;
}