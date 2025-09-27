#pragma once
#include "color.hpp"
#include <string>

struct Material {
    std::string name;
    Color3 kd{0.8f}; // diffuse color
    Color3 ks{0.0f}; // specular color
    float ka = 0.0f; // ambient factor
    float shininess = 32.0f; // specular power
    float reflectivity = 0.0f; // mirror reflectivity factor
};
