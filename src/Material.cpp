#include "Material.h"
#include "BSDFCookTorrance.h"

Material::Material(const string &bsdfType, const vector<float> *params) {
    if (bsdfType == "Lambertian") {
        if (params->size() >= 3) {
            bsdf = make_shared<BSDFLambertian>(vec3((*params)[0], (*params)[1], (*params)[2]));
        }
        else {
            bsdf = make_shared<BSDFLambertian>(vec3(1.0f));
        }
    }
    else if (bsdfType == "Specular") {
        if (params->size() >= 3) {
            bsdf = make_shared<BSDFSpecular>(vec3((*params)[0], (*params)[1], (*params)[2]));
        }
        else {
            bsdf = make_shared<BSDFSpecular>(vec3(1.0f));
        }
    }
    else if (bsdfType == "Refraction") {
        if (params->size() >= 4) {
            bsdf = make_shared<BSDFRefraction>(vec3((*params)[0], (*params)[1], (*params)[2]), (*params)[3]);
        }
        else {
            bsdf = make_shared<BSDFRefraction>(vec3(1.0f), 1.5f);
        }
    }
    else if (bsdfType == "CookTorrance") {
        if (params->size() >= 5) {
            bsdf = make_shared<BSDFCookTorrance>(vec3((*params)[0], (*params)[1], (*params)[2]), (*params)[3], (*params)[4]);
        }
        else {
            bsdf = make_shared<BSDFCookTorrance>(vec3(1.0f), 10.0f, 1.5f);
        }
    }
    else {
        cerr << "ERROR: Unsupported material.\n";
        exit(1);
    }
}