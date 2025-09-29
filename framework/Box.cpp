#include "Box.hpp"
#include <algorithm>
#include <limits>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>

Box::Box(const std::string& name, const Material* mat, const glm::vec3& bmin, const glm::vec3& bmax)
    : Shape(name, mat), bmin_(bmin), bmax_(bmax) {
}

bool Box::intersect(const Ray& ray, HitPoint& hp, const glm::mat4& parentWorldToLocal, const glm::mat3& parentNormalMat) const {
    glm::mat4 W2L = this->invTransform() * parentWorldToLocal;
    Ray r = transformRay(ray, W2L);

    glm::vec3 invD(1.0f / r.dir.x, 1.0f / r.dir.y, 1.0f / r.dir.z);
    glm::vec3 t0 = (bmin_ - r.origin) * invD;
    glm::vec3 t1 = (bmax_ - r.origin) * invD;

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
    if (tmax < tmin) return false;

    float t = tmin;
    if (t >= hp.t) return false;

    glm::vec3 p_local = r.origin + t * r.dir;

    glm::vec3 n_local(0.0f);
    const float eps = 1e-3f;
    if (std::abs(p_local.x - bmin_.x) < eps) n_local = glm::vec3(-1, 0, 0);
    else if (std::abs(p_local.x - bmax_.x) < eps) n_local = glm::vec3(1, 0, 0);
    else if (std::abs(p_local.y - bmin_.y) < eps) n_local = glm::vec3(0, -1, 0);
    else if (std::abs(p_local.y - bmax_.y) < eps) n_local = glm::vec3(0, 1, 0);
    else if (std::abs(p_local.z - bmin_.z) < eps) n_local = glm::vec3(0, 0, -1);
    else n_local = glm::vec3(0, 0, 1);

    glm::vec4 p_world4 = glm::inverse(W2L) * glm::vec4(p_local, 1.0f);
    glm::vec3 p_world = glm::vec3(p_world4);

    // Точное t вдоль исходного луча
    float t_world = glm::dot(p_world - ray.origin, ray.dir);
    if (t_world <= 1e-5f || t_world >= hp.t) return false;

    glm::mat3 Nmat = glm::mat3(glm::transpose(W2L));
    glm::vec3 n_world = glm::normalize(Nmat * n_local);

    hp.t = t_world;
    hp.position = p_world;
    hp.normal = n_world;
    hp.material = material_;
    hp.hit = true;
    return true;
}
