#pragma once
#include "Shape.hpp"

class Sphere : public Shape {
public:
    Sphere(const std::string& name, const Material* mat, const glm::vec3& center, float radius);
    bool intersect(const Ray& ray, HitPoint& hp, const glm::mat4& parentWorldToLocal, const glm::mat3& parentNormalMat) const override;
private:
    glm::vec3 c_;
    float r_;
};
