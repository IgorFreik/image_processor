#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <cstdlib>
#include <tuple>
#include <fstream>
#include <ios>
#include <stdio.h>
#include <vector>

struct Image {
    struct Color {
        const static int MAX_VALUE = 255;

        Color() {}
        Color(unsigned char x, unsigned char y, unsigned char z) {
            r = x;
            g = y;
            b = z;
        }
        unsigned char r;
        unsigned char g;
        unsigned char b;
        Image::Color& operator=(const Color& other)=default;
        Image::Color operator+(const Color& other) {
            return Color(r + other.r, g + other.g, b + other.b);
        }
    };
    int height;
    int width;
    std::vector<std::vector<Color>> data;

    Image()=default;
};
