#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "BSDF.h"

class Material {
public:
    Material() {
        bsdf = make_shared<BSDFLambertian>(vec3(1.0f));
    }
    Material(const string &bsdfType, const vector<float> *params) {
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
    }
    virtual ~Material() {}
    shared_ptr<BSDF> bsdf;
};

#endif