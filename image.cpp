#include "image.hh"

#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace ray {

    Color::Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue)
    {}

    bool operator==(const Color& lhs, const Color& rhs) {
        if (lhs.r != rhs.r) return false;
        if (lhs.g != rhs.g) return false;
        if (lhs.b != rhs.b) return false;
        return true;
    }
    
    bool operator!=(const Color& lhs, const Color& rhs) {
        if (lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b) return false;
        return true;
    }

    Image::Image(int width, int height) {
        height_ = height;
        width_ = width;
        pixels = static_cast<Color*>(std::calloc(width * height, sizeof(Color)));
    }

    Image::~Image() {
        std::free(pixels);
    }

    void Image::save(std::string name) {
        std::ofstream file;
        file.open(name);
        file << "P3\n";
        file << width_ << "\n";
        file << height_ << "\n";
        file << "255\n";

        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                file << int(pixels[i * width_ + j].r) << ' ';
                file << int(pixels[i * width_ + j].g) << ' ';
                file << int(pixels[i * width_ + j].b) << ' ';
            }

            file << '\n';
        }

        file.close();
    }
}
