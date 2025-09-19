#pragma once
#include <string>
#include <vector>
#include "pixel.hpp"

namespace ppm {
    bool writeP6(const std::string& filename, int width, int height, const std::vector<Pixel>& pixels);
}
