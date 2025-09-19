#include "renderer.hpp"
#include "color.hpp"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>

Renderer::Renderer(const Scene& scene, const RenderSettings& settings)
    : scene_(scene), settings_(settings) {
}

static Ray make_primary_ray(const Camera& cam, int x, int y, int W, int H) {
    float aspect = float(W) / float(H);
    float fov = cam.fov_deg * 3.14159265358979323846f / 180.0f;
    float px = ((x + 0.5f) / float(W) * 2.0f - 1.0f) * std::tan(fov * 0.5f) * aspect;
    float py = (1.0f - (y + 0.5f) / float(H) * 2.0f) * std::tan(fov * 0.5f);
    glm::vec3 dir = glm::normalize(glm::vec3(px, py, -1.0f)); // camera looks along -Z
    return Ray{ cam.eye, dir };
}

bool Renderer::occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const {
    Ray shadowRay{ p + 1e-3f * toLight, toLight };
    HitPoint hp;
    for (const auto& obj : scene_.objects) {
        obj->intersect(shadowRay, hp);
        if (hp.hit && hp.t < dist) return true;
    }
    return false;
}

Color3 Renderer::shade(const HitPoint& hp, const glm::vec3& wo) const {
    const Material* m = hp.material;
    Color3 color(0.0f);

    // Ambient
    color += m->ka * m->kd;

    // For each point light
    for (const auto& L : scene_.lights) {
        glm::vec3 wi = L.position - hp.position;
        float dist = glm::length(wi);
        if (dist <= 0.0f) continue;
        wi /= dist;

        // Shadow
        if (occluded(hp.position, wi, dist)) continue;

        float ndotl = std::max(0.0f, glm::dot(hp.normal, wi));
        // Diffuse
        Color3 diff = m->kd * ndotl;

        // Specular (Blinn-Phong)
        glm::vec3 h = glm::normalize(wi + wo);
        float ndoth = std::max(0.0f, glm::dot(hp.normal, h));
        Color3 spec = m->ks * std::pow(ndoth, m->shininess);

        // Light intensity falloff 1/r^2
        float falloff = 1.0f / (dist * dist);
        color += (diff + spec) * L.intensity * falloff;
    }

    return color;
}

std::vector<Pixel> Renderer::render() const {
    int W = settings_.width;
    int H = settings_.height;
    color_buffer.assign(W * H, Pixel{});

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Ray ray = make_primary_ray(scene_.camera, x, y, W, H);
            HitPoint best;
            for (const auto& obj : scene_.objects) {
                obj->intersect(ray, best);
            }
            Color3 hdr(0.0f);
            if (best.hit) {
                glm::vec3 wo = glm::normalize(-ray.dir);
                hdr = shade(best, wo);
            }
            else {
                hdr = scene_.background;
            }
            // Tonemap to LDR
            Color3 ldr = tonemap(hdr);
            color_buffer[y * W + x] = Pixel::fromColorLDR(ldr);
        }
    }
    return color_buffer;
}
