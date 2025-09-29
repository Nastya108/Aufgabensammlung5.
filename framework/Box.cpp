#include "Box.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <glm/glm.hpp>        // Basic types
#include <glm/geometric.hpp>  // normalize

Box::Box(const std::string& name, const Material* mat, const glm::vec3& bmin, const glm::vec3& bmax)
    : Shape(name, mat), bmin_(bmin), bmax_(bmax) {
}

bool Box::intersect(const Ray& ray, HitPoint& hp) const {

    // Transform ray to local coordinate system
        Ray localRay = transformRay(ray, inv_transform_);

    // Slab method for axis-aligned box intersection
        glm::vec3 invD(1.0f / localRay.dir.x, 1.0f / localRay.dir.y, 1.0f / localRay.dir.z);
        glm::vec3 t0 = (bmin_ - localRay.origin) * invD;
        glm::vec3 t1 = (bmax_ - localRay.origin) * invD;

        // Find min and max t values for each axis
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

    // Find intersection range
    float tmin = std::max(std::max(std::max(tminv.x, tminv.y), tminv.z), 1e-4f);
    float tmax = std::min(std::min(tmaxv.x, tmaxv.y), tmaxv.z);

    // Check for valid intersection
    if (tmax < tmin || tmin >= hp.t) return false;

    // Calculate local hit point and normal
    glm::vec3 localHit = localRay.origin + tmin * localRay.dir;

    // Determine face normal based on hit position
    glm::vec3 localNormal(0.0f);
    const float eps = 1e-3f;
    if (std::abs(localHit.x - bmin_.x) < eps) localNormal = glm::vec3(-1, 0, 0);
    else if (std::abs(localHit.x - bmax_.x) < eps) localNormal = glm::vec3(1, 0, 0);
    else if (std::abs(localHit.y - bmin_.y) < eps) localNormal = glm::vec3(0, -1, 0);
    else if (std::abs(localHit.y - bmax_.y) < eps) localNormal = glm::vec3(0, 1, 0);
    else if (std::abs(localHit.z - bmin_.z) < eps) localNormal = glm::vec3(0, 0, -1);
    else localNormal = glm::vec3(0, 0, 1);

    // Transform hit point and normal back to world space
    glm::vec4 worldHit4 = world_transform_ * glm::vec4(localHit, 1.0f);
    glm::vec4 worldNormal4 = glm::transpose(glm::inverse(world_transform_))
        * glm::vec4(localNormal, 0.0f);

    glm::vec3 worldHit = glm::vec3(worldHit4);
    glm::vec3 worldNormal = glm::normalize(glm::vec3(worldNormal4));

    // Store hit point data
    hp.t = glm::length(worldHit - ray.origin);
    hp.position = worldHit;
    hp.normal = worldNormal;
    hp.material = material_;
    hp.hit = true;

    return true;
}
