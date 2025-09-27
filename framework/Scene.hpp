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
    glm::vec3 eye{ 0.0f, 0.0f, 0.0f };    // Позиція камери
    glm::vec3 dir{ 0.0f, 0.0f, -1.0f };   // Напрямок погляду (нормалізований)
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };     // Вектор "вгору"
    float fov_deg = 60.0f;                // Горизонтальний кут огляду
};

struct Scene {
    std::vector<std::unique_ptr<Shape>> objects;

    // Âëàäåíèå ìàòåðèàëàìè
    std::vector<std::unique_ptr<Material>> materials_owned;

    // Ïëîñêèå óêàçàòåëè íà ìàòåðèàëû — ÄËß ÑÎÂÌÅÑÒÈÌÎÑÒÈ ñ âàøèì raytracer.cpp (scene.materials)
    std::vector<Material*> materials;

    std::unordered_map<std::string, const Material*> material_by_name;
    std::vector<PointLight> lights;

    Camera camera;
    Color3 background{ 0.0f, 0.0f, 0.0f };

    const Material* findMaterial(const std::string& name) const {
        auto it = material_by_name.find(name);
        return (it == material_by_name.end()) ? nullptr : it->second;
    }

    // Äîáàâëåíèå è îäíîâðåìåííàÿ ñèíõðîíèçàöèÿ â scene.materials
    Material* addMaterial(std::unique_ptr<Material> m) {
        auto ptr = m.get();
        material_by_name[m->name] = ptr;
        materials_owned.emplace_back(std::move(m));
        materials.push_back(ptr);
        return ptr;
    }
};
