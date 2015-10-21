#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "BRDF.h"

class Material {
public:
    Material(const string &brdfType) {
        if (brdfType == "Lambertian") {
            brdf = make_shared<BRDFLambertian>();
        }
    }
    virtual ~Material() {}
    shared_ptr<BRDF> brdf;
};

#endif