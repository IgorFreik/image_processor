#include "image.h"

struct BMPHeader {
    // BMP Header
    unsigned char FileType[2];
    unsigned char Size[4];
    unsigned char Reserved[4];
    unsigned char PixelOffset[4];
    // DIB Header
    unsigned char HSize[4];
    unsigned char Width[4];
    unsigned char Height[4];

    static int SwapEnds32bit(const unsigned char x[4]) {
        return x[0] + (x[1] << 8) + (x[2] << 16) + (x[3] << 24);
    }
};

struct BMP {
    static Image read(const char *file_name);

    static void write(const Image &im, const char* path);
};
