#include "io.h"

Image BMP::read(const char *file_name) {
    Image im;
    BMPHeader header;
    std::ifstream input{file_name, std::ios_base::binary};
    input.read(reinterpret_cast<char*>(header.FileType), 2);
    input.read(reinterpret_cast<char*>(header.Size), 4);
    input.read(reinterpret_cast<char*>(header.Reserved), 4);
    input.read(reinterpret_cast<char*>(header.PixelOffset), 4);
    input.read(reinterpret_cast<char*>(header.HSize), 4);
    input.read(reinterpret_cast<char*>(header.Width), 4);
    input.read(reinterpret_cast<char*>(header.Height), 4);
    input.seekg(BMPHeader::SwapEnds32bit(header.PixelOffset), input.beg);
    im.width = BMPHeader::SwapEnds32bit(header.Width);
    im.height = BMPHeader::SwapEnds32bit(header.Height);
    const int tail_size = (4 - (3 * im.width) % 4) % 4;
    im.data.push_back(std::vector<Image::Color>(0));
    for (size_t y = 0; y < im.height; ++y) {
        for (size_t x = 0; x < im.width; ++x) {
            unsigned char pixel[3];
            input.read(reinterpret_cast<char*>(pixel), 3);
            im.data[y].push_back(Image::Color(pixel[2], pixel[1], pixel[0]));
        }
        input.ignore(tail_size);
        im.data.push_back(std::vector<Image::Color>(0));
    }
    im.data.pop_back();
    input.close();
    return im;
}

void BMP::write(const Image &im, const char* path) {
    const int HeaderSize = 54;
    const int TailSize = (4 - (3 * im.width) % 4) % 4;
    const int FileSize = HeaderSize + 3 * im.width * im.height + TailSize * im.height;
    unsigned char tempBMPHeader[HeaderSize];

    tempBMPHeader[0] = 'B'; //File type
    tempBMPHeader[1] = 'M';

    tempBMPHeader[2] = FileSize;
    tempBMPHeader[3] = FileSize >> 8;
    tempBMPHeader[4] = FileSize >> 16;
    tempBMPHeader[5] = FileSize >> 24;

    tempBMPHeader[6] = 0; // reserved
    tempBMPHeader[7] = 0;
    tempBMPHeader[8] = 0;
    tempBMPHeader[9] = 0;

    tempBMPHeader[10] = HeaderSize;
    tempBMPHeader[11] = 0;
    tempBMPHeader[12] = 0;
    tempBMPHeader[13] = 0;

    tempBMPHeader[14] = 40; // DIB size
    tempBMPHeader[15] = 0;
    tempBMPHeader[16] = 0;
    tempBMPHeader[17] = 0;

    tempBMPHeader[18] = im.width; // width
    tempBMPHeader[19] = im.width >> 8;
    tempBMPHeader[20] = im.width >> 16;
    tempBMPHeader[21] = im.width >> 24;

    tempBMPHeader[22] = im.height; // height
    tempBMPHeader[23] = im.height >> 8;
    tempBMPHeader[24] = im.height >> 16;
    tempBMPHeader[25] = im.height >> 24;

    tempBMPHeader[26] = 1; // 1 color plane
    tempBMPHeader[27] = 0;

    tempBMPHeader[28] = 24; //24 bit
    tempBMPHeader[29] = 0;

    tempBMPHeader[30] = 0;
    tempBMPHeader[31] = 0;
    tempBMPHeader[32] = 0;
    tempBMPHeader[33] = 0;
    tempBMPHeader[34] = 0;
    tempBMPHeader[35] = 0;
    tempBMPHeader[36] = 0;
    tempBMPHeader[37] = 0;
    tempBMPHeader[38] = 0;
    tempBMPHeader[39] = 0;
    tempBMPHeader[40] = 0;
    tempBMPHeader[41] = 0;
    tempBMPHeader[42] = 0;
    tempBMPHeader[43] = 0;
    tempBMPHeader[44] = 0;
    tempBMPHeader[45] = 0;
    tempBMPHeader[46] = 0;
    tempBMPHeader[47] = 0;
    tempBMPHeader[48] = 0;
    tempBMPHeader[49] = 0;
    tempBMPHeader[50] = 0;
    tempBMPHeader[51] = 0;
    tempBMPHeader[52] = 0;
    tempBMPHeader[53] = 0;

    std::ofstream output{path, std::ios::out | std::ios_base::binary};
    const char emptyPixel[3] = {0, 0, 0};
    output.write((char*) tempBMPHeader, sizeof(tempBMPHeader));
    for (size_t y = 0; y < im.height; ++y) {
        for (size_t x = 0; x < im.width; ++x) {
            unsigned char pixel[3] = {im.data[y][x].b, im.data[y][x].g, im.data[y][x].r};
            output.write(reinterpret_cast<char*>(pixel), 3);
        }
        output.write(emptyPixel, TailSize);
    }
}