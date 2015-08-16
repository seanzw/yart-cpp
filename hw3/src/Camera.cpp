#include "Camera.h"

Camera::Camera(const vec3 &eye,
    const vec3 &center,
    const vec3&up,
    float fovradian,
    int width,
    int height
    ) : e(eye), tantheta(tan(fovradian / 2.0f)), w(float(width)), h(float(height)) {

    // Build the local coordinate.
    z = normalize(e - center);
    y = normalize(up - dot(z, up) * z);
    x = cross(y, z);
}

Ray Camera::genRay(int row, int col) {
    float r = (((float)row + 0.5f) / h - 0.5f) * 2.0f * tantheta;
    float c = (((float)col + 0.5f) / w - 0.5f) * 2.0f * tantheta * w / h;
    vec3 direction = normalize(c * x + r * y - z);
    return Ray(e, direction);
}