#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "Ray.h"

class Camera {
public:
    Camera(const vec3 &eye,
        const vec3 &center,
        const vec3&up,
        float fovradian,
        int width,
        int height
        );
    ~Camera() {}

    Ray genRay(int row, int col);
    
private:
    vec3 e, x, y, z;
    float w, h;
    float tantheta;
};

#endif