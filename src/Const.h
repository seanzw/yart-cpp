#ifndef CONST_HEADER
#define CONST_HEADER

#include <cstdio>
#define DEBUG(...) printf(__VA_ARGS__)
//#define DEBUG(...)

#include <iostream>

#include <vector>
#include <memory>
using namespace std;

#include "glm\glm.hpp"
using namespace glm;

#define CONST_FAR 1000000.0f
#define CONST_NEAR 0.001f
#define PI 3.14159265f


struct Material {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Hit {
    float t;
    vec3 point;
    vec3 normal;
    Material m;
};

inline vec3 applyMatrix(const mat4 &trans, const vec3 &v) {
    vec4 temp = vec4(v, 1.0f);
    temp = trans * temp;
    temp = temp / temp[3];
    return vec3(temp);
}

#endif