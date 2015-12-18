#include "SphericalEnvironmentLight.h"

SphericalEnvironmentLight::SphericalEnvironmentLight(
	shared_ptr<Texture> &texture,
	float power
	)
    : Light(), texture(texture), power(power) {

    r = texture->getWidth() / 2.0f / PI;
    area = 4 * PI * r * r;
    Material m;
    sphere = make_unique<Sphere>(m, vec3(0.0f), r, mat4(1.0f));
}

SphericalEnvironmentLight::~SphericalEnvironmentLight() {

}

Intersection SphericalEnvironmentLight::intersect(const Ray &r) const {
    Intersection hit = sphere->intersect(r);
    if (hit.type == INTERSECTION_NONE) {
        return hit;
    }

    hit.type = INTERSECTION_LIGHT;
    hit.normal = -hit.normal;
    hit.intersectable = this;
    hit.pos = INTERSECTION_OUT;
    return hit;
}

pair<Ray, float> SphericalEnvironmentLight::genShadowRay(const Intersection &hit) const {
    
    vec3 point = get<0>(samplePoint());
    vec3 toLight = point - hit.point;
    vec3 direction = normalize(toLight);

    float p = pdfRay(hit, direction);
    return make_pair(Ray(hit.point, direction, CONST_NEAR, length(toLight)), p);
}

tuple<vec3, vec3, float> SphericalEnvironmentLight::samplePoint() const {
    float theta = Sampler::sample1D(0.0f, PI);
    float phi = Sampler::sample1D(0.0f, 2.0f * PI);
    vec3 point(sinf(theta) * sinf(phi), cosf(theta), sinf(theta) * cosf(phi));
    vec3 normal = -point;
    point *= r;
    return make_tuple(point, normal, 1.0f / area);
}

float SphericalEnvironmentLight::pdfPoint(const vec3 &point) const {
    return 1.0f / area;
}

float SphericalEnvironmentLight::pdfRay(const Intersection &hit, const vec3 &direction) const {

    Ray r(hit.point, direction, CONST_NEAR, CONST_FAR);

    // Intersection test with the sphere.
    Intersection temp = intersect(r);
    if (temp.type != INTERSECTION_LIGHT) {
        return 0.0f;
    }

    float cosThetaI = clamp(dot(hit.normal, direction), 0.01f, 1.0f);
    float cosThetaO = clamp(dot(temp.normal, -direction), 0.01f, 1.0f);
    return temp.t * temp.t / (area * cosThetaI * cosThetaO);
}

vec3 SphericalEnvironmentLight::Le(const vec3 &point) const {
    vec3 unit = normalize(point);
    float theta = acosf(unit.y);
    float phi;
    if (unit.x >= 0.0f) {
        if (unit.z >= 0.0f) {
            phi = atanf(unit.x / unit.z);
        }
        else {
            phi = atanf(unit.x / unit.z) + PI;
        }
    } else {
        if (unit.z >= 0.0f) {
            phi = atanf(unit.x / unit.z) + 2 * PI;
        }
        else {
            phi = atanf(unit.x / unit.z) + PI;
        }
    }
    float s = phi / (2 * PI);
    float t = theta / PI;
    return texture->map(s, t) * power;
}



