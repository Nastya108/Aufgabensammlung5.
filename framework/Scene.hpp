#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>
#include "Shape.hpp"
#include "Material.hpp"
#include "color.hpp"

struct PointLight {
    glm::vec3 position{ 0.0f };
    Color3 intensity{ 1.0f };
};

struct Camera {
    glm::vec3 eye{0,0,0};
    glm::vec3 dir{0,0,-1};
    glm::vec3 up{0,1,0};
    float fov_deg = 60.0f;
};

struct Scene {
    // Root of the scene graph
    std::shared_ptr<Shape> root;

    // Materials store (ownership)
    std::vector<std::unique_ptr<Material>> materials_owned;
    std::vector<Material*> materials; // for compatibility

    std::unordered_map<std::string, const Material*> material_by_name;
    std::vector<PointLight> lights;

    Camera camera;
    Color3 background{0.0f, 0.0f, 0.0f};

    const Material* findMaterial(const std::string& name) const {
        auto it = material_by_name.find(name);
        return (it == material_by_name.end()) ? nullptr : it->second;
    }
    Material* addMaterial(std::unique_ptr<Material> m) {
        auto ptr = m.get();
        material_by_name[m->name] = ptr;
        materials_owned.emplace_back(std::move(m));
        materials.push_back(ptr);
        return ptr;
    }
};
