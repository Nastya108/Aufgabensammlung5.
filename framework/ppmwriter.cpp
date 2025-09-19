#include "ppmwriter.hpp"
#include <fstream>

bool ppm::writeP6(const std::string& filename, int width, int height, const std::vector<Pixel>& pixels){
    if (width <= 0 || height <= 0 || static_cast<int>(pixels.size()) != width*height) return false;
    std::ofstream ofs(filename, std::ios::binary);
    if(!ofs) return false;
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for(const auto& p : pixels){
        ofs.put(static_cast<char>(p.r));
        ofs.put(static_cast<char>(p.g));
        ofs.put(static_cast<char>(p.b));
    }
    return true;
}
