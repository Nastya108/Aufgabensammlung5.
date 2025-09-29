#pragma once
#include <vector>
#include "Scene.hpp"
#include "pixel.hpp"

class Renderer {
public:
    // Constructor with scene and settings
    Renderer(const Scene& scene, const RenderSettings& settings);

    // Compatibility constructor for raytracer.cpp: {width, height, scene}
    Renderer(int width, int height, const Scene& scene)
        : scene_(scene) {
        settings_.width = width;
        settings_.height = height;
        settings_.samples_per_pixel = 1;
    }

    // Render: returns buffer and fills public color_buffer for compatibility
    std::vector<Pixel> render() const;

    // Public buffer - for compatibility with existing code
    mutable std::vector<Pixel> color_buffer;

private:
    const Scene& scene_;
    RenderSettings settings_;

    // Shading and illumination calculations
    Color3 shade(const HitPoint& hp, const glm::vec3& wo) const;

    // Shadow testing
    bool occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const;

    Color3 trace(const Ray& ray, int depth) const; //trace() function declaration
  
    // Creates primary ray for given pixel coordinates
    static Ray make_primary_ray(const Camera& cam, float x, float y, int W, int H);
};

