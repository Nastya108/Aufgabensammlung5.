#pragma once
#include <vector>
#include "Scene.hpp"
#include "pixel.hpp"

struct RenderSettings {
    int width = 800;
    int height = 600;
    int max_bounces = 0; // primary rays only (no GI)
};

class Renderer {
public:
    // Старый конструктор
    Renderer(const Scene& scene, const RenderSettings& settings);

    // Конструктор для совместимости с вашим raytracer.cpp: {width, height, scene}
    Renderer(int width, int height, const Scene& scene)
        : scene_(scene) {
        settings_.width = width;
        settings_.height = height;
    }

    // Рендер: возвращает буфер и ПОПУТНО заполняет публичный color_buffer для совместимости
    std::vector<Pixel> render() const;

    // Публичный буфер — для совместимости с кодом (scene/raytracer.cpp)
    mutable std::vector<Pixel> color_buffer;

private:
    const Scene& scene_;
    RenderSettings settings_;
    Color3 shade(const HitPoint& hp, const glm::vec3& wo) const;
    bool occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const;
};
