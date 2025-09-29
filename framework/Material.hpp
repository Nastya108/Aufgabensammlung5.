#pragma once
#include "color.hpp"
#include <string>

struct Material {
    std::string name;

    // локальные параметры освещения
    Color3 kd{ 0.8f };   // diffuse
    Color3 ks{ 0.0f };   // specular
    float  ka = 0.0f;  // ambient factor
    float  shininess = 32.0f;

    // зеркальность/прозрачность
    float  reflect = 0.0f; // [0..1] вклад зеркального отражения
    float  refract = 0.0f; // [0..1] вклад преломления (прозрачности)
    float  ior = 1.0f; // показатель преломления (air~1.0, glass~1.5)

    // НОВОЕ: двустороннее освещение (для пола/стен и тонких панелей)
    bool   two_sided = false;
};
