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
    // Ñòàðûé êîíñòðóêòîð
    Renderer(const Scene& scene, const RenderSettings& settings);

    // Êîíñòðóêòîð äëÿ ñîâìåñòèìîñòè ñ âàøèì raytracer.cpp: {width, height, scene}
    Renderer(int width, int height, const Scene& scene)
        : scene_(scene) {
        settings_.width = width;
        settings_.height = height;
    }

    // Ðåíäåð: âîçâðàùàåò áóôåð è ÏÎÏÓÒÍÎ çàïîëíÿåò ïóáëè÷íûé color_buffer äëÿ ñîâìåñòèìîñòè
    std::vector<Pixel> render() const;

    // Ïóáëè÷íûé áóôåð — äëÿ ñîâìåñòèìîñòè ñ êîäîì (scene/raytracer.cpp)
    mutable std::vector<Pixel> color_buffer;

private:
    const Scene& scene_;
    RenderSettings settings_;
    Color3 shade(const HitPoint& hp, const glm::vec3& wo) const;
    bool occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const;

    Color3 trace(const Ray& ray, int depth) const; //trace() function declaration

};

