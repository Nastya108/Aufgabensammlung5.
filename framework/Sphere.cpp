#include "Sphere.hpp"
#include <cmath>
#include <glm/glm.hpp>        // базовые типы/операции
#include <glm/geometric.hpp>  // dot, normalize

Sphere::Sphere(const std::string& name, const Material* mat, const glm::vec3& center, float radius)
    : Shape(name, mat), c_(center), r_(radius) {
}

bool Sphere::intersect(const Ray& ray, HitPoint& hp) const {
    // Ray-sphere intersection
    glm::vec3 oc = ray.origin - c_;
    float b = glm::dot(oc, ray.dir);
    float c = glm::dot(oc, oc) - r_ * r_;
    float disc = b * b - c;
    if (disc < 0.0f) return false;
    float s = std::sqrt(disc);
    float t1 = -b - s;
    float t2 = -b + s;
    float t = (t1 > 1e-4f) ? t1 : ((t2 > 1e-4f) ? t2 : 1e30f);
    if (t >= hp.t) return false;

    hp.t = t;
    hp.position = ray.origin + t * ray.dir;
    hp.normal = glm::normalize(hp.position - c_);
    hp.material = material_;
    hp.hit = true;
    return true;
}
