#include "AreaLight.h"
#include "Sampler.h"

AreaLight::AreaLight(vec3 c, vec3 color, vec3 n, float r)
    : Light(color), center(c), normal(n), radius(r) {

    area = PI * radius * radius;

    // Build the local coordinate.
    if (abs(normal.x) > 1.0f - CONST_EPSILON) {
        xUnit = vec3(-normal.y, normal.x, 0.0f);
    }
    else {
        xUnit = vec3(0.0f, -normal.z, normal.y);
    }
    xUnit = normalize(xUnit);
    yUnit = cross(normal, xUnit);
}

Intersection AreaLight::intersect(const Ray &r) const {

    Intersection ret(NULL, this, CONST_FAR, INTERSECTION_NONE);

    // Intersection test with the circle.
    float dotDN = dot(normal, r.d);
    if (dotDN > 0.0f) {
        return ret;
    }

    vec3 toCenter = center - r.o;

    float t = dot(toCenter, normal) / dotDN;

    if (t <= r.tmin || t >= r.tmax) {
        return ret;
    }

    vec3 point = r.o + t * r.d;
    vec3 temp = center - point;
    float toCenterSqure = dot(temp, temp);
    if (toCenterSqure > radius * radius) {
        return ret;
    }

    ret.normal = normal;
    ret.point = point;
    ret.t = t;
    ret.type = INTERSECTION_LIGHT;
    return ret;
}

pair<Ray, float> AreaLight::genShadowRay(const Intersection &hit) const {
    
    vec3 point = get<0>(samplePoint());

    // Get the direction of the ray.
    vec3 toLight = point - hit.point;
    vec3 direction = normalize(toLight);

    // Get the pdf of this ray.
    float p = pdfRay(hit, direction);
    return make_pair(Ray(hit.point, direction, CONST_NEAR, length(toLight)), p);
}

tuple<vec3, vec3, float> AreaLight::samplePoint() const {

    // Randomly sample a point on the disk.
    // Randomly sample a point on the disk.
    auto sample = Sampler::uniformSampleCircle();
    float r = sample.first * radius;
    float theta = sample.second;

    // Get the point on the circle.
    float x = r * cosf(theta);
    float y = r * sinf(theta);

    // Project it into world coordinate.
    vec3 point = center + x * xUnit + y * yUnit;

    return make_tuple(point, normal, 1.0f / area);
}

vec3 AreaLight::Le() const {
    return c;
}

/**
 * The pdf is the probability of projected solid angle.
 * p = (d ^ 2) / (S * cos(thetaI) * cos(thetaO));
 */
float AreaLight::pdfRay(const Intersection &hit, const vec3 &direction) const {

    Ray r(hit.point, direction, CONST_NEAR, CONST_FAR);

    // Intersection test with the circle.
    Intersection temp = intersect(r);
    if (temp.type != INTERSECTION_LIGHT) {
        return 0.0f;
    }

    float cosThetaI = clamp(dot(hit.normal, direction), 0.01f, 1.0f);
    float cosThetaO = clamp(dot(normal, -direction), 0.01f, 1.0f);
    return temp.t * temp.t / (area * cosThetaI * cosThetaO);
}