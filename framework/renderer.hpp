#pragma once
#include <vector>
#include "Scene.hpp"
#include "pixel.hpp"

struct RenderSettings {
    int  width = 800;
    int  height = 600;
    int  samples_per_pixel = 1;
    int  max_depth = 4;        // предел рекурсии для отражений/преломлений
};

class Renderer {
public:
    Renderer(const Scene& scene, const RenderSettings& settings);
    Renderer(int width, int height, const Scene& scene)
        : scene_(scene) {
        settings_.width = width; settings_.height = height;
    }

    std::vector<Pixel> render() const;
    mutable std::vector<Pixel> color_buffer;

    // fov в камере — горизонтальный
    static Ray make_primary_ray(const Camera& cam, float x, float y, int W, int H);

private:
    const Scene& scene_;
    RenderSettings   settings_;

    Color3 shade(const HitPoint& hp, const glm::vec3& wo) const; // локальное освещение
    bool   occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const;

    // рекурсивная трассировка с отражением/преломлением
    Color3 trace(const Ray& ray, int depth = 0) const;
};
