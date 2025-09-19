#pragma once
#include <algorithm>
#include <glm/vec3.hpp>

// Базовый цвет (linear RGB)
using Color3 = glm::vec3;
// Алиас для совместимости с вашим window.hpp, который ждёт Color
using Color = Color3;

inline Color3 clamp01(const Color3& c) {
    return Color3(
        std::max(0.0f, std::min(1.0f, c.r)),
        std::max(0.0f, std::min(1.0f, c.g)),
        std::max(0.0f, std::min(1.0f, c.b))
    );
}

// Тонмаппинг: cldr = chdr / (chdr + 1)
inline Color3 tonemap(const Color3& hdr) {
    return Color3(
        hdr.r / (hdr.r + 1.0f),
        hdr.g / (hdr.g + 1.0f),
        hdr.b / (hdr.b + 1.0f)
    );
}
