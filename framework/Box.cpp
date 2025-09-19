#include "Box.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <glm/glm.hpp>        // базовые типы
#include <glm/geometric.hpp>  // normalize

Box::Box(const std::string& name, const Material* mat, const glm::vec3& bmin, const glm::vec3& bmax)
    : Shape(name, mat), bmin_(bmin), bmax_(bmax) {
}

bool Box::intersect(const Ray& ray, HitPoint& hp) const {
    // Slab method
    glm::vec3 invD(1.0f / ray.dir.x, 1.0f / ray.dir.y, 1.0f / ray.dir.z);
    glm::vec3 t0 = (bmin_ - ray.origin) * invD;
    glm::vec3 t1 = (bmax_ - ray.origin) * invD;

    // Без glm::min/max: компонентно через std::min/std::max
    glm::vec3 tminv(
        std::min(t0.x, t1.x),
        std::min(t0.y, t1.y),
        std::min(t0.z, t1.z)
    );
    glm::vec3 tmaxv(
        std::max(t0.x, t1.x),
        std::max(t0.y, t1.y),
        std::max(t0.z, t1.z)
    );

    float tmin = std::max(std::max(std::max(tminv.x, tminv.y), tminv.z), 1e-4f);
    float tmax = std::min(std::min(tmaxv.x, tmaxv.y), tmaxv.z);

    if (tmax < tmin || tmin >= hp.t) return false;

    // Compute normal at hit
    float t = tmin;
    glm::vec3 p = ray.origin + t * ray.dir;
    glm::vec3 n(0.0f);
    const float eps = 1e-3f;
    if (std::abs(p.x - bmin_.x) < eps) n = glm::vec3(-1, 0, 0);
    else if (std::abs(p.x - bmax_.x) < eps) n = glm::vec3(1, 0, 0);
    else if (std::abs(p.y - bmin_.y) < eps) n = glm::vec3(0, -1, 0);
    else if (std::abs(p.y - bmax_.y) < eps) n = glm::vec3(0, 1, 0);
    else if (std::abs(p.z - bmin_.z) < eps) n = glm::vec3(0, 0, -1);
    else n = glm::vec3(0, 0, 1);

    hp.t = t;
    hp.position = p;
    hp.normal = glm::normalize(n);
    hp.material = material_;
    hp.hit = true;
    return true;
}
