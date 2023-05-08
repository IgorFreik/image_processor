#pragma once
#include "image.h"

struct Crop {
    static void Apply(Image& image, int width, int height);
};

class Grayscale {
public:
    static void Apply(Image& image);
};

class Negative {
public:
    static void Apply(Image& image);
};

class MatrixFilter {
public:
    static void Apply(Image& image, const std::vector<std::vector<int>>& matrix);

    static Image::Color FilteredPixel(Image& image, int x, int y, const std::vector<std::vector<int>>& matrix);
};

class Sharpening : MatrixFilter {
public:
    static void Apply(Image& image);
};

class EdgeDetection : MatrixFilter {
public:
    static void Apply(Image& image, double threshold);
};
