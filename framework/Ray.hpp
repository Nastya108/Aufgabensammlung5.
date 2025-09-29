#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

struct Ray {
    glm::vec3 origin;    // Ray origin point
    glm::vec3 dir;       // Normalized direction vector
};

// Transforms ray from one coordinate system to another
inline Ray transformRay(const Ray& r, const glm::mat4& m) {
    glm::vec4 o = m * glm::vec4(r.origin, 1.0f);
    glm::vec4 d = m * glm::vec4(r.dir, 0.0f);
    Ray out;
    out.origin = glm::vec3(o);
    out.dir = glm::normalize(glm::vec3(d));
    return out;
}