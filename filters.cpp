#include "filters.h"

void Crop::Apply(Image& image, int width, int height) {
    const int cropped_width = image.width - width;
    const int cropped_height = image.height - height;
    if (cropped_height > 0) {
        for (int i = 0; i < cropped_height; ++i) {
            image.data.pop_back();
        }
        image.height -= cropped_height;
    }
    if (cropped_width > 0) {
        for (std::vector<Image::Color>& line : image.data) {
            for (int i = 0; i < cropped_width; ++i) {
                line.pop_back();
            }
        }
        image.width -= cropped_width;
    }
}



void Grayscale::Apply(Image& image) {
    for (std::vector<Image::Color>& line : image.data) {
        for (Image::Color& pixel : line) {
            unsigned char grayed = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
            pixel = Image::Color(grayed, grayed, grayed);
        }
    }
}


void Negative::Apply(Image& image) {
    for (std::vector<Image::Color>& line : image.data) {
        for (Image::Color& pixel : line) {
            pixel = Image::Color(255 - pixel.r, 255 - pixel.g, 255 - pixel.b);
        }
    }
}

void MatrixFilter::Apply(Image& image, const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<Image::Color>> new_data(image.width);
    new_data.push_back(std::vector<Image::Color>(image.width));
    new_data.reserve(image.height);
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            new_data[y].push_back(FilteredPixel(image, x, y, matrix));
        }
    }
    image.data = new_data;
}

unsigned char my_double_to_unsignchar_clamp(double value) {
    if (value < 0) {
        return 0;
    }
    if (value > 255) {
        return 255;
    }
    return static_cast<unsigned char>(value);
}

Image::Color MatrixFilter::FilteredPixel(Image& image, int x, int y, const std::vector<std::vector<int>>& matrix) {
    double new_pix[3];

    for (int a = y - 1; a <= y + 1; ++a) {
        for (int b = x - 1; b <= x + 1; ++b) {
            size_t yn = std::clamp(a, 0, image.height - 1);
            size_t xn = std::clamp(b, 0, image.width - 1);
            new_pix[0] += matrix[a - (y - 1)][b - (x - 1)] * (static_cast<int>(image.data[yn][xn].r));
            new_pix[1] += matrix[a - (y - 1)][b - (x - 1)] * (static_cast<int>(image.data[yn][xn].g));
            new_pix[2] += matrix[a - (y - 1)][b - (x - 1)] * (static_cast<int>(image.data[yn][xn].b));
        }
    }
    unsigned char r = my_double_to_unsignchar_clamp(new_pix[0]);
    unsigned char g = my_double_to_unsignchar_clamp(new_pix[1]);
    unsigned char b = my_double_to_unsignchar_clamp(new_pix[2]);
    return Image::Color(r, g, b);
}


void Sharpening::Apply(Image& image) {
    MatrixFilter::Apply(image, {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}});
}

void EdgeDetection::Apply(Image& image, double threshold) {
    Grayscale::Apply(image);
    MatrixFilter::Apply(image, {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}});
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            if (image.data[y][x].r > threshold) {
                image.data[y][x] = Image::Color(255, 255, 255);
            } else {
                image.data[y][x] = Image::Color(0, 0, 0);
            }
        }
    }
}
