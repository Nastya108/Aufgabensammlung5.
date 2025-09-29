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
    Color3 intensity{ 1.0f }; // RGB intensity/power
};

struct Camera {
    glm::vec3 eye{ 0.0f, 0.0f, 0.0f };    // camera position
    glm::vec3 dir{ 0.0f, 0.0f, -1.0f };   // view direction (normalized)
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };     // up vector 
    float fov_deg = 60.0f;                // horizontal field of view
};

struct RenderSettings {
    int width = 800;
    int height = 600;
    int max_bounces = 5;
    int samples_per_pixel = 1;  // Anti-aliasing samples per pixel
};

struct Scene {
    std::vector<std::unique_ptr<Shape>> objects;

    // Material ownership
    std::vector<std::unique_ptr<Material>> materials_owned;

    // Material pointers for compatibility
    std::vector<Material*> materials;

    std::unordered_map<std::string, const Material*> material_by_name;
    std::vector<PointLight> lights;

    Camera camera;
    Color3 background{ 0.0f, 0.0f, 0.0f };

    // Find material by name
    const Material* findMaterial(const std::string& name) const {
        auto it = material_by_name.find(name);
        return (it == material_by_name.end()) ? nullptr : it->second;
    }

    // Add material and sync with materials list
    Material* addMaterial(std::unique_ptr<Material> m) {
        auto ptr = m.get();
        material_by_name[m->name] = ptr;
        materials_owned.emplace_back(std::move(m));
        materials.push_back(ptr);
        return ptr;
    }
};
