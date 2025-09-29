#include "Sphere.hpp"
#include <cmath>
#include <glm/glm.hpp>        // Basic types/operations
#include <glm/geometric.hpp>  // dot, normalize

Sphere::Sphere(const std::string& name, const Material* mat, const glm::vec3& center, float radius)
    : Shape(name, mat), c_(center), r_(radius) {
}

bool Sphere::intersect(const Ray& ray, HitPoint& hp) const {

    // Transform ray to local coordinate system
    Ray localRay = transformRay(ray, inv_transform_);

    // Sphere intersection in local coordinates
    glm::vec3 oc = localRay.origin - c_;
    float b = glm::dot(oc, localRay.dir);
    float c = glm::dot(oc, oc) - r_ * r_;
    float disc = b * b - c;
    if (disc < 0.0f) return false;

    // Find intersection distance
    float s = std::sqrt(disc);
    float t1 = -b - s;
    float t2 = -b + s;
    float t = (t1 > 1e-4f) ? t1 : ((t2 > 1e-4f) ? t2 : 1e30f);
    if (t >= hp.t) return false;

    // Calculate local hit point and normal
    glm::vec3 localHit = localRay.origin + t * localRay.dir;
    glm::vec3 localNormal = glm::normalize(localHit - c_);

    // Transform hit point and normal back to world space
    glm::vec4 worldHit4 = world_transform_ * glm::vec4(localHit, 1.0f);
    glm::vec4 worldNormal4 = glm::transpose(glm::inverse(world_transform_))
        * glm::vec4(localNormal, 0.0f);

    // Fill HitPoint structure
    glm::vec3 worldHit = glm::vec3(worldHit4);
    glm::vec3 worldNormal = glm::normalize(glm::vec3(worldNormal4));

    hp.t = glm::length(worldHit - ray.origin);  // Distance in world coordinates
    hp.position = worldHit;
    hp.normal = worldNormal;
    hp.material = material_;
    hp.hit = true;

    return true;
}

