#pragma once
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.hpp"
#include "HitPoint.hpp"
#include "Material.hpp"

class Shape {
public:
    Shape(std::string name, const Material* mat)
        : name_(std::move(name)), material_(mat),
        localToWorld_(1.0f), worldToLocal_(1.0f) {
    }

    virtual ~Shape() = default;

    // Intersect with accumulated transforms from parent
    virtual bool intersect(const Ray& ray,
        HitPoint& hp,
        const glm::mat4& parentWorldToLocal,
        const glm::mat3& parentNormalMat) const = 0;

    const std::string& name() const { return name_; }
    const Material* material() const { return material_; }
    void setMaterial(const Material* m) { material_ = m; }

    // Node transform
    void setTransform(const glm::mat4& m) {
        localToWorld_ = m;
        worldToLocal_ = glm::inverse(m);
    }
    const glm::mat4& transform()    const { return localToWorld_; }
    const glm::mat4& invTransform() const { return worldToLocal_; }

protected:
    std::string name_;
    const Material* material_ = nullptr;
    glm::mat4 localToWorld_;
    glm::mat4 worldToLocal_;
};
