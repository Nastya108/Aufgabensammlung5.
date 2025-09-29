#include "renderer.hpp"
#include "Composite.hpp"
#include "color.hpp"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>
#include <algorithm>

Renderer::Renderer(const Scene& scene, const RenderSettings& settings)
    : scene_(scene), settings_(settings) {
}

// Горизонтальный FOV (cam.fov_deg) -> вертикальный вычисляем из аспект-отношения
Ray Renderer::make_primary_ray(const Camera& cam, float x, float y, int W, int H) {
    float aspect = float(W) / float(H);
    float fovx = cam.fov_deg * 3.14159265358979323846f / 180.0f;
    float fovy = 2.0f * std::atan(std::tan(fovx * 0.5f) / aspect);

    float sx = ((x + 0.5f) / float(W)) * 2.0f - 1.0f;
    float sy = 1.0f - ((y + 0.5f) / float(H)) * 2.0f;

    glm::vec3 forward = glm::normalize(cam.dir);
    glm::vec3 right = glm::normalize(glm::cross(forward, cam.up));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));

    glm::vec3 dir = glm::normalize(forward
        + sx * std::tan(fovx * 0.5f) * right
        + sy * std::tan(fovy * 0.5f) * up);
    return Ray{ cam.eye, dir };
}

// Прозрачные материалы не считаем «заслоняющими» свет
bool Renderer::occluded(const glm::vec3& p, const glm::vec3& toLight, float dist) const {
    if (!scene_.root) return false;
    const float bias = 2e-3f;
    Ray shadowRay{ p + bias * toLight, toLight };
    HitPoint hp;
    scene_.root->intersect(shadowRay, hp, glm::mat4(1.0f), glm::mat3(1.0f));
    if (hp.hit && hp.t < dist) {
        const Material* m = hp.material;
        if (m && m->refract > 0.0f) {
            return false; // свет проходит (упрощённо)
        }
        return true; // непрозрачный объект затеняет
    }
    return false;
}

Color3 Renderer::shade(const HitPoint& hp, const glm::vec3& wo) const {
    const Material* m = hp.material;
    Color3 color(0.0f);

    // ambient
    color += m->ka * m->kd;

    for (const auto& L : scene_.lights) {
        glm::vec3 wi = L.position - hp.position;
        float dist = glm::length(wi);
        if (dist <= 0.0f) continue;
        wi /= dist;

        if (occluded(hp.position, wi, dist)) continue;

        // Двустороннее освещение по флагу материала
        glm::vec3 n = hp.normal;
        if (m->two_sided && glm::dot(n, wi) < 0.0f) {
            n = -n;
        }

        float ndotl = std::max(0.0f, glm::dot(n, wi));
        Color3 diff = m->kd * ndotl;

        glm::vec3 h = glm::normalize(wi + wo);
        float ndoth = std::max(0.0f, glm::dot(n, h));
        Color3 spec = m->ks * std::pow(ndoth, m->shininess);

        float falloff = 1.0f / (dist * dist);
        color += (diff + spec) * L.intensity * falloff;
    }
    return color;
}

static inline glm::vec3 reflectDir(const glm::vec3& v, const glm::vec3& n) {
    return glm::normalize(v - 2.0f * glm::dot(v, n) * n);
}

Color3 Renderer::trace(const Ray& ray, int depth) const {
    if (depth > settings_.max_depth) return Color3(0.0f);
    if (!scene_.root)               return scene_.background;

    HitPoint best;
    scene_.root->intersect(ray, best, glm::mat4(1.0f), glm::mat3(1.0f));
    if (!best.hit) return scene_.background;

    const Material* m = best.material;
    glm::vec3 wo = glm::normalize(-ray.dir);

    // локальное освещение
    Color3 local = shade(best, wo);

    // коэффициенты смешивания
    float kr = std::clamp(m->reflect, 0.0f, 1.0f);
    float kt = std::clamp(m->refract, 0.0f, 1.0f);
    float kLocal = std::max(0.0f, 1.0f - kr - kt);

    Color3 result = kLocal * local;

    const float bias = 2e-3f;

    // отражение
    if (kr > 0.0f) {
        glm::vec3 rdir = reflectDir(-wo, best.normal);
        Ray rr{ best.position + bias * rdir, rdir };
        result += kr * trace(rr, depth + 1);
    }

    // преломление
    if (kt > 0.0f) {
        glm::vec3 n = best.normal;
        float n1 = 1.0f;      // воздух
        float n2 = m->ior;    // материал
        glm::vec3 v = -wo;  // входящее направление (в сторону поверхности)

        float cosi = glm::dot(v, n);
        bool outside = cosi >= 0.0f;
        float eta = outside ? (n1 / n2) : (n2 / n1);
        glm::vec3 nn = outside ? n : -n;

        // Пробуем преломление
        glm::vec3 tdir = glm::refract(v, nn, eta);
        if (glm::dot(tdir, tdir) > 0.0f) {
            tdir = glm::normalize(tdir);
            Ray tr{ best.position - bias * nn + bias * tdir, tdir };
            result += kt * trace(tr, depth + 1);
        }
        else {
            // Полное внутреннее отражение: вся энергия передачи переходит в отражение
            glm::vec3 rdir = reflectDir(v, nn);
            Ray rr{ best.position + bias * rdir, rdir };
            result += kt * trace(rr, depth + 1);
        }
    }

    return result;
}

std::vector<Pixel> Renderer::render() const {
    int W = settings_.width;
    int H = settings_.height;
    int spp = std::max(1, settings_.samples_per_pixel);

    color_buffer.assign(W * H, Pixel{});

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Color3 acc(0.0f);
            for (int s = 0; s < spp; ++s) {
                Ray ray = make_primary_ray(scene_.camera, float(x), float(y), W, H);
                acc += trace(ray, 0);
            }
            Color3 hdr = acc / float(spp);
            Color3 ldr = tonemap(hdr); // cldr = chdr / (chdr + 1)
            color_buffer[y * W + x] = Pixel::fromColorLDR(ldr);
        }
    }
    return color_buffer;
}
