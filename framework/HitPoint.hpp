#pragma once
#include <glm/vec3.hpp>
#include <string>

// Represents the result of a ray-shape intersection
struct HitPoint {
    bool        hit{ false };        // True if the ray hit something
    float       distance{ 0.0f };    // Distance from ray origin to intersection (t)
    std::string name{};              // Name of the intersected shape
    glm::vec3   color{ 0.0f };       // Color of the intersected shape
    glm::vec3   position{ 0.0f };    // Position of the intersection point
    glm::vec3   direction{ 0.0f };   // Normalized direction at the hit point
};
