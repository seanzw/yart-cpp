// Transform.cpp: implementation of the Transform class.

#include "Transform.h"

//Please implement the following functions:
using namespace glm;

// Some helper functions.
static mat3 matrixCross3(const vec3 &u) {
    return mat3(0, u.z, -u.y,
        -u.z, 0, u.x,
        u.y, -u.x, 0);
}

static mat3 outerProduct3(const vec3 &u, const vec3 &v) {
    return mat3(v.x * u, v.y * u, v.z * u);
}

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
    // YOUR CODE FOR HW1 HERE
    float theta = radians(degrees);

    // normalize the axis
    vec3 axis_normalized = normalize(axis);

    mat3 matrix = cos(theta) * mat3() +
        sin(theta) * matrixCross3(axis_normalized) +
        (1 - cos(theta)) * outerProduct3(axis_normalized, axis_normalized);

    // You will change this return call
    return matrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {

    // YOUR CODE FOR HW1 HERE
    mat3 matrix = rotate(degrees, up);
    eye = matrix * eye;

}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {

    // YOUR CODE FOR HW1 HERE
    vec3 axis = cross(eye, up);
    mat3 matrix = rotate(degrees, axis);
    eye = matrix * eye;
    up = matrix * up;

}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
    // YOUR CODE FOR HW1 HERE

    // Define local frame.
    vec3 z = normalize(eye);
    vec3 y = normalize(up - dot(up, z) * z);
    vec3 x = cross(y, z);

    mat4 rotate_mat = transpose(mat4(vec4(x, 0.0f),
        vec4(y, 0.0f),
        vec4(z, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)));

    mat4 translate_mat = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(-eye, 1.0f));

    return rotate_mat * translate_mat;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
