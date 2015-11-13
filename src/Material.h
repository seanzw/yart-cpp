#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "BRDF.h"

class Material {
public:
    Material() {
        brdf = make_shared<BRDFLambertian>(vec3(1.0f));
    }
    Material(const string &brdfType, const vector<float> *params) {
        if (brdfType == "Lambertian") {
            if (params->size() >= 3) {
                brdf = make_shared<BRDFLambertian>(vec3((*params)[0], (*params)[1], (*params)[2]));
            }
            else {
                brdf = make_shared<BRDFLambertian>(vec3(1.0f));
            }
        }
        else if (brdfType == "Specular") {
            if (params->size() >= 3) {
                brdf = make_shared<BRDFSpecular>(vec3((*params)[0], (*params)[1], (*params)[2]));
            }
            else {
                brdf = make_shared<BRDFSpecular>(vec3(1.0f));
            }
        }
    }
    virtual ~Material() {}
    shared_ptr<BRDF> brdf;
};

#endif