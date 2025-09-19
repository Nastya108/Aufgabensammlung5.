#pragma once
#include "color.hpp"
#include <cstdint>

struct Pixel {
    uint8_t r, g, b;
    Pixel(): r(0), g(0), b(0) {}
    Pixel(uint8_t rr, uint8_t gg, uint8_t bb): r(rr), g(gg), b(bb) {}
    static Pixel fromColorLDR(const Color3& c){
        Color3 cm = clamp01(c);
        return Pixel(
            static_cast<uint8_t>(255.0f * cm.r + 0.5f),
            static_cast<uint8_t>(255.0f * cm.g + 0.5f),
            static_cast<uint8_t>(255.0f * cm.b + 0.5f)
        );
    }
};
