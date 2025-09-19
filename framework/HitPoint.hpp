#pragma once
#include <glm/vec3.hpp>
struct Material;

struct HitPoint {
    float t = 1e30f;
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f, 0.0f, 1.0f};
    const Material* material = nullptr;
    bool hit = false;
};
