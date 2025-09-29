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
        : name_(std::move(name)), material_(mat) {
        world_transform_ = glm::mat4(1.0f);
        inv_transform_ = glm::mat4(1.0f);
    }
    virtual ~Shape() = default;

    // Pure virtual intersection function
    virtual bool intersect(const Ray& ray, HitPoint& hp) const = 0;

    // Accessors
    const std::string& name() const { return name_; }
    const Material* material() const { return material_; }
    void setMaterial(const Material* m){ material_ = m; }

    // Transformation management
    void setTransform(const glm::mat4& m) {
        world_transform_ = m;
        inv_transform_ = glm::inverse(m);
    }
    const glm::mat4& transform() const { return world_transform_; }
    const glm::mat4& invTransform() const { return inv_transform_; }

protected:
    std::string name_;
    const Material* material_ = nullptr;

    // Transformation matrices for object positioning
    glm::mat4 world_transform_;
    glm::mat4 inv_transform_;
};
