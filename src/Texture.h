#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>
#include "Const.h"

class Texture {
public:
    Texture(const string &fn);
    ~Texture();

    vec3 map(float s, float t) const;

    inline int getWidth() const { return width; };


private:
    float *texture;
    int width;
    int height;

};

#endif