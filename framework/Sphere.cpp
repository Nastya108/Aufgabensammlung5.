#include "Sphere.hpp"
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <cmath>

Sphere::Sphere(const std::string& name, const Material* mat, const glm::vec3& center, float radius)
    : Shape(name, mat), c_(center), r_(radius) {
}

bool Sphere::intersect(const Ray& ray, HitPoint& hp, const glm::mat4& parentWorldToLocal, const glm::mat3& parentNormalMat) const {
    // Составляем матрицу world->local для этого узла
    glm::mat4 W2L = this->invTransform() * parentWorldToLocal;
    Ray rlocal = transformRay(ray, W2L);

    // Пересечение в локальном пространстве
    glm::vec3 oc = rlocal.origin - c_;
    float b = glm::dot(oc, rlocal.dir);
    float c = glm::dot(oc, oc) - r_ * r_;
    float disc = b * b - c;
    if (disc < 0.0f) return false;
    float s = std::sqrt(disc);
    float t1 = -b - s;
    float t2 = -b + s;
    float t = (t1 > 1e-4f) ? t1 : ((t2 > 1e-4f) ? t2 : 1e30f);
    if (t >= hp.t) return false;

    // Локальные hit-позиция и нормаль
    glm::vec3 p_local = rlocal.origin + t * rlocal.dir;
    glm::vec3 n_local = glm::normalize(p_local - c_);

    // Переводим точку в мир
    glm::vec4 p_world4 = glm::inverse(W2L) * glm::vec4(p_local, 1.0f);
    glm::vec3 p_world = glm::vec3(p_world4);

    // Правильный параметр вдоль исходного луча (направление нормировано)
    float t_world = glm::dot(p_world - ray.origin, ray.dir);
    if (t_world <= 1e-5f || t_world >= hp.t) return false;

    // Трансформируем нормаль
    glm::mat3 Nmat = glm::mat3(glm::transpose(W2L)); // для инвертированной матрицы
    glm::vec3 n_world = glm::normalize(Nmat * n_local);

    hp.t = t_world;
    hp.position = p_world;
    hp.normal = n_world;
    hp.material = material_;
    hp.hit = true;
    return true;
}
