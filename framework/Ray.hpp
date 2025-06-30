#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>   // for glm::normalize

// Represents a ray in 3D space
struct Ray {
    glm::vec3 origin{ 0.0f, 0.0f, 0.0f };        // Ray origin point
    glm::vec3 direction{ 0.0f, 0.0f, -1.0f };    // normalized direction
};