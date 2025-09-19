#pragma once
#include "Shape.hpp"

// Axis-aligned box defined by min and max
class Box : public Shape {
public:
    Box(const std::string& name, const Material* mat, const glm::vec3& bmin, const glm::vec3& bmax);
    bool intersect(const Ray& ray, HitPoint& hp) const override;
private:
    glm::vec3 bmin_, bmax_;
};
