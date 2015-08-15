// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include "stdio.h"

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
mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
    // YOUR CODE FOR HW1 HERE

    // Define local frame.
    vec3 z = normalize(eye - center);
    vec3 y = normalize(up);
    vec3 x = normalize(cross(y, z));
    y = cross(z, x);

    mat4 rotate_mat = transpose(mat4(vec4(x, 0.0f),
        vec4(y, 0.0f),
        vec4(z, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)));

    mat4 translate_mat = mat4(vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec4(-eye, 1.0f));

    mat4 ret = rotate_mat * translate_mat;

    // for (int i = 0; i < 4; ++i) {
    //     for (int j = 0; j < 4; j++)
    //         printf("%.2f ", (ret)[j][i]);
    //     printf("\n");
    //     }
    // printf("\n");

    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar) {
    mat4 ret = mat4(0.0f);

    float d = 1.0f / tan(radians(fovy / 2.0f));

    ret[0][0] = d / aspect;
    ret[1][1] = d;
    ret[2][2] = (zFar + zNear) / (zNear - zFar);
    ret[2][3] = -1.0f;
    ret[3][2] = 2.0f * zFar * zNear / (zNear - zFar);

    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
    mat4 ret = mat4(sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
    mat4 ret = mat4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        tx, ty, tz, 1.0f);

    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
