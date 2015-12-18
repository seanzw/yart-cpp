#ifndef FILM_HEADER
#define FILM_HEADER

#include "Const.h"

typedef unsigned char uchar;

class Film {
public:
    Film(int width, int height);
    Film(const Film &other);
    Film &operator=(const Film &other);
    ~Film();

    void expose(const vec3 &color, int row, int col);
    void rinse(string fn);

private:
    int w, h;
    uchar *buff;
};

#endif