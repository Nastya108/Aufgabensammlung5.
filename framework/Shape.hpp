#pragma once
#include <string>
#include <memory>
#include <glm/vec3.hpp>
#include "Ray.hpp"
#include "HitPoint.hpp"
#include "Material.hpp"

class Shape {
public:
    Shape(std::string name, const Material* mat): name_(std::move(name)), material_(mat) {}
    virtual ~Shape() = default;

    virtual bool intersect(const Ray& ray, HitPoint& hp) const = 0;

    const std::string& name() const { return name_; }
    const Material* material() const { return material_; }
    void setMaterial(const Material* m){ material_ = m; }

protected:
    std::string name_;
    const Material* material_ = nullptr;
};
