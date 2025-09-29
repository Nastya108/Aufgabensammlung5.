#pragma once
#include "Shape.hpp"
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

class Composite : public Shape {
public:
    Composite(const std::string& name)
    : Shape(name, /*material*/ nullptr) {}

    void addChild(const std::shared_ptr<Shape>& child){
        children_.push_back(child);
    }

    // Intersect by recursively testing children with composed transforms
    bool intersect(const Ray& ray,
                   HitPoint& hp,
                   const glm::mat4& parentWorldToLocal,
                   const glm::mat3& parentNormalMat) const override;

    // For convenience we keep a local transform on each node (already in base)
private:
    std::vector<std::shared_ptr<Shape>> children_;
};
