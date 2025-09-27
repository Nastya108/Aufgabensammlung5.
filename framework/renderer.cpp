#include "renderer.hpp"
#include "color.hpp"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>

Renderer::Renderer(const Scene& scene, const RenderSettings& settings)
    : scene_(scene), settings_(settings) {
}

static Ray make_primary_ray(const Camera& cam, int x, int y, int W, int H) {
    // --- 1. Базис камери ---
    glm::vec3 n = glm::normalize(-cam.dir);       // напрямок "назад"
    glm::vec3 u = glm::normalize(glm::cross(cam.up, n)); // вправо
    glm::vec3 v = glm::cross(n, u);              // уточнений вектор вгору

    // --- 2. Горизонтальний FOV і аспект ---
    float aspect = float(W) / float(H);
    float fov_rad = cam.fov_deg * 3.14159265358979323846f / 180.0f;

    // --- 3. Нормалізовані координати пікселя ---
    float px = ((x + 0.5f) / float(W) * 2.0f - 1.0f) * std::tan(fov_rad * 0.5f) * aspect;
    float py = (1.0f - (y + 0.5f) / float(H) * 2.0f) * std::tan(fov_rad * 0.5f);

    // --- 4. Напрямок променя у системі світу ---
    glm::vec3 dir_world = glm::normalize(u * px + v * py - n);

    return Ray{ cam.eye, dir_world };
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

    // ===== reflection implementation =====
    Color3 reflectedColor(0.0f);
    if (best.material->reflectivity > 0.0f && depth < MAX_DEPTH) {
        // calculate reflection direction using the formula: R = I - 2*(I·N)*N
        // where I is the incident vector (normalized ray direction)
        // and N is the surface normal
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
    // reflectivity = 0.0 -> full local color, reflectivity = 
    float refl = best.material->reflectivity;
    return localColor * (1.0f - refl) + reflectedColor * refl;
}

Color3 Renderer::shade(const HitPoint& hp, const glm::vec3& wo) const {
    const Material* m = hp.material;
    Color3 color(0.0f);

    // ambient
    color += m->ka * m->kd;

    // for each point light
    for (const auto& L : scene_.lights) {
        glm::vec3 wi = L.position - hp.position;
        float dist = glm::length(wi);
        if (dist <= 0.0f) continue;
        wi /= dist;

        // shadow
        if (occluded(hp.position, wi, dist)) continue;

        float ndotl = std::max(0.0f, glm::dot(hp.normal, wi));
        // diffuse
        Color3 diff = m->kd * ndotl;

        // specular (Blinn-Phong)
        glm::vec3 h = glm::normalize(wi + wo);
        float ndoth = std::max(0.0f, glm::dot(hp.normal, h));
        Color3 spec = m->ks * std::pow(ndoth, m->shininess);

        // light intensity falloff 1/r^2
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
            // create primary ray for current pixel
            Ray ray = make_primary_ray(scene_.camera, x, y, W, H);

            // trace ray through scene with reflection support
            Color3 hdr = trace(ray, 0);

            // apply tone mapping to convert HDR to LDR
            Color3 ldr = tonemap(hdr);
            color_buffer[y * W + x] = Pixel::fromColorLDR(ldr);
        }
    }

    return color_buffer;
}