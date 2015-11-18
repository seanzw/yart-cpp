#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "BSDF.h"

class Material {
public:
    Material() {
        bsdf = make_shared<BSDFLambertian>(vec3(1.0f));
    }
    Material(const string &bsdfType, const vector<float> *params);
    virtual ~Material() {}
    shared_ptr<BSDF> bsdf;
};

#endif