#ifndef SPHERICAL_ENVIRONMENT_LIGHT_HEADER
#define SPHERICAL_ENVIRONMENT_LIGHT_HEADER

#include "Light.h"
#include "Texture.h"
#include "Sphere.h"

class SphericalEnvironmentLight : public Light {
public:
    SphericalEnvironmentLight(shared_ptr<Texture> &texture, float power);
    virtual ~SphericalEnvironmentLight();

    virtual Intersection intersect(const Ray &r) const;
    virtual pair<Ray, float> genShadowRay(const Intersection &hit) const;
    virtual tuple<vec3, vec3, float> samplePoint() const;

    virtual float pdfPoint(const vec3 &point) const;
    virtual float pdfRay(const Intersection &hit, const vec3 &direction) const;

    virtual vec3 Le(const vec3 &point) const;

private:
    float r;
	float power;
    float area;
    unique_ptr<Sphere> sphere;
    shared_ptr<Texture> texture;
};

#endif