#include "Texture.h"
#include "FreeImage/FreeImage.h"

typedef unsigned char BYTE;

Texture::Texture(const string &fn) {

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fn.c_str(), 0);//Automatocally detects the format(from over 20 formats!)
    FIBITMAP* image = FreeImage_Load(format, fn.c_str());

    //FIBITMAP* temp = image;
    //image = FreeImage_ConvertTo32Bits(temp);
    //FreeImage_Unload(temp);

    width = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);
    DEBUG("Loading Texture %d x %d\n", width, height);

    texture = new float[width * height * 3];

    BYTE* pixeles = (BYTE*)FreeImage_GetBits(image);

    //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            texture[(i * width + j) * 3 + 0] = (float)pixeles[((height - i - 1) * width + j) * 3 + 2] / 255.0f;
            texture[(i * width + j) * 3 + 1] = (float)pixeles[((height - i - 1) * width + j) * 3 + 1] / 255.0f;
            texture[(i * width + j) * 3 + 2] = (float)pixeles[((height - i - 1) * width + j) * 3 + 0] / 255.0f;
        }
    }
}

Texture::~Texture() {
    delete[] texture;
}

vec3 Texture::map(float s, float t) const {
    int col = (int)(s * width);
    int row = (int)(t * height);
    if (col == width) col -= 1;
    if (row == height) row -= 1;
    int idx = (row * width + col) * 3;
    return vec3(texture[idx], texture[idx + 1], texture[idx + 2]);
}