#include "renderer.hpp"
#include "color.hpp"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>

Renderer::Renderer(const Scene& scene, const RenderSettings& settings)
    : scene_(scene), settings_(settings) {
}
//creates primary ray through pixel (x,y) for perspective camera
Ray Renderer::make_primary_ray(const Camera& cam, float x, float y, int W, int H) {

    // build camera coordinate system
    glm::vec3 n = glm::normalize(-cam.dir);       // camera forward (-Z)
    glm::vec3 u = glm::normalize(glm::cross(cam.up, n)); // camera right (X)
    glm::vec3 v = glm::cross(n, u);             // camera up (Y)

    // perspective projection setup
    float aspect = float(W) / float(H);
    float fov_rad = cam.fov_deg * 3.14159265358979323846f / 180.0f;

    // convert pixel to camera space coordinates
    float px = ((x + 0.5f) / float(W) * 2.0f - 1.0f) * std::tan(fov_rad * 0.5f) * aspect;
    float py = (1.0f - (y + 0.5f) / float(H) * 2.0f) * std::tan(fov_rad * 0.5f);

    // transform to world space direction
    glm::vec3 dir_world = glm::normalize(u * px + v * py - n);

    return Ray{ cam.eye, dir_world };
}

// Check if point is occluded from light source (shadow testing)
bool Renderer::occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const {
    Ray shadowRay{ p + 1e-3f * toLight, toLight };
    HitPoint hp;
    for (const auto& obj : scene_.objects) {
        obj->intersect(shadowRay, hp);
        if (hp.hit && hp.t < dist) return true;
    }
    return false;
}
// recursive ray tracing function with reflection support
Color3 Renderer::trace(const Ray& ray, int depth) const {
    const int MAX_DEPTH = 5;
    if (depth > MAX_DEPTH) {
        return scene_.background;
    }

    // find the closest object intersection along the ray
    HitPoint best;
    best.t = std::numeric_limits<float>::max();
    for (const auto& obj : scene_.objects) {
        obj->intersect(ray, best);
    }

    // return background color if no intersection found
    if (!best.hit) {
        return scene_.background;
    }

    // view (wo) direction what is the opposite to ray direction
    glm::vec3 wo = glm::normalize(-ray.dir);

    // calculating local illumination
    Color3 localColor = shade(best, wo);

    // Reflection implementation
    Color3 reflectedColor(0.0f);
    if (best.material->reflectivity > 0.0f && depth < MAX_DEPTH) {

        // Calculate reflection direction: R = I - 2*(I·N)*N
        glm::vec3 I = glm::normalize(ray.dir);
        glm::vec3 R = I - 2.0f * glm::dot(I, best.normal) * best.normal;

        // reflection ray with slight offset to avoid self-intersection
        Ray reflectedRay{
            best.position + 1e-4f * best.normal,
            glm::normalize(R)
        };

        // recursively trace the reflection ray
        reflectedColor = trace(reflectedRay, depth + 1);
    }


    // blend local color and reflected color based on reflectivity coefficient
    float refl = best.material->reflectivity;
    return localColor * (1.0f - refl) + reflectedColor * refl;
}

// Calculate local illumination using Blinn-Phong model
Color3 Renderer::shade(const HitPoint& hp, const glm::vec3& wo) const {
    const Material* m = hp.material;
    Color3 color(0.0f);

    // ambient component
    color += m->ka * m->kd;

    // Process each point light
    for (const auto& L : scene_.lights) {
        glm::vec3 wi = L.position - hp.position;
        float dist = glm::length(wi);
        if (dist <= 0.0f) continue;
        wi /= dist;

        // shadow check
        if (occluded(hp.position, wi, dist)) continue;

        float ndotl = std::max(0.0f, glm::dot(hp.normal, wi));
        // diffuse componend (Lambertian)
        Color3 diff = m->kd * ndotl;

        // specular component (Blinn-Phong)
        glm::vec3 h = glm::normalize(wi + wo);
        float ndoth = std::max(0.0f, glm::dot(hp.normal, h));
        Color3 spec = m->ks * std::pow(ndoth, m->shininess);

        // light intensity falloff 1/r^2 (inverse square law)
        float falloff = 1.0f / (dist * dist);
        color += (diff + spec) * L.intensity * falloff;
    }

    return color;
}

// Main rendering function with anti-aliasing support
std::vector<Pixel> Renderer::render() const {
    int W = settings_.width;
    int H = settings_.height;
    color_buffer.assign(W * H, Pixel{});

    int samples = settings_.samples_per_pixel;

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Color3 accumulated_color(0.0f);

            // Anti-aliasing: multiple samples per pixel
            for (int s = 0; s < samples; ++s) {
                float sample_x, sample_y;

                if (samples == 1) {
                    // Single sample at pixel center (original behavior)
                    sample_x = x + 0.5f;
                    sample_y = y + 0.5f;
                }
                else {
                    // 4 samples with jittering
                    float jitter_x = (s % 2 == 0) ? 0.25f : 0.75f;
                    float jitter_y = (s < 2) ? 0.25f : 0.75f;
                    sample_x = x + jitter_x;
                    sample_y = y + jitter_y;
                }

                Ray ray = make_primary_ray(scene_.camera, sample_x, sample_y, W, H);
                accumulated_color += trace(ray, 0);
            }

            // Average the color samples and apply tone mapping
            Color3 hdr = accumulated_color / static_cast<float>(samples);
            Color3 ldr = tonemap(hdr);
            color_buffer[y * W + x] = Pixel::fromColorLDR(ldr);
        }
    }

    return color_buffer;
}