#include <cstring>      /* For memcpy. */
#include "Film.h"
#include "FreeImage/FreeImage.h"

Film::Film(int width, int height) : w(width), h(height) {
    buff = new uchar[w * h * 3];
}

Film::Film(const Film &other) {
    DEBUG("FILM COPIED\n");
    w = other.w;
    h = other.h;
    buff = new uchar[w * h * 3];
    memcpy((void *)buff, (void *)other.buff, w * h * 3);
}

Film &Film::operator=(const Film &other) {
    DEBUG("FILM COPIED\n");
    w = other.w;
    h = other.h;
    buff = new uchar[w * h * 3];
    memcpy((void *)buff, (void *)other.buff, w * h * 3);
    return *this;
}

Film::~Film() {
    DEBUG("RELEASE FILM\n");
    delete[] buff;
    buff = NULL;
}

void Film::expose(const vec3 &color, int row, int col) {
    int offset = row * w * 3 + col * 3;
    int r = uchar(color[0] * 255.0f);
    int g = uchar(color[1] * 255.0f);
    int b = uchar(color[2] * 255.0f);
    /*DEBUG("EXPOSE ROW = %d COL = %d r %d g %d b %d\n", row, col, r, g, b);
    DEBUG("OFFSET = %d\n", offset);*/
    buff[offset + 2] = r;
    buff[offset + 1] = g;
    buff[offset + 0] = b;
}

void Film::rinse(string fn) {

    FreeImage_Initialise();

    BYTE *pixels = new BYTE[w * h * 3];
    memcpy((void *)pixels, (void *)buff, w * h * 3);

    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24,
        0x0000FF, 0x00FF00, 0xFF0000, false);
    FreeImage_Save(FIF_PNG, img, fn.c_str(), 0);

    delete[] pixels;
    FreeImage_DeInitialise();

}