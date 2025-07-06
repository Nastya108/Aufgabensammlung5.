#include "Box.hpp"
#include <cmath>

// Constructor
Box::Box(const std::string& name, const glm::vec3& color,
    const glm::vec3& min, const glm::vec3& max)
    : Shape(name, color), min_(min), max_(max) {
}

// Compute surface area of the box
double Box::area() const {
    double dx = max_.x - min_.x;
    double dy = max_.y - min_.y;
    double dz = max_.z - min_.z;

    return 2.0 * (dx * dy + dy * dz + dz * dx);
}

// Compute volume of the box
double Box::volume() const {
    double dx = max_.x - min_.x;
    double dy = max_.y - min_.y;
    double dz = max_.z - min_.z;

    return dx * dy * dz;
}

// Print info about the box
std::ostream& Box::print(std::ostream& os) const {
    Shape::print(os);
    os << ", Min: (" << min_.x << ", " << min_.y << ", " << min_.z << ")"
        << ", Max: (" << max_.x << ", " << max_.y << ", " << max_.z << ")";
    return os;
}

// Ray-box intersection method
HitPoint Box::intersect(Ray const& ray) const
{
    HitPoint hp;  // Default: no hit

    const float INF = std::numeric_limits<float>::infinity();
    float tmin = -INF;
    float tmax = INF;

    // Update tmin and tmax for each axis (slab method)
    auto update = [&](float minA, float maxA, float oA, float dA) -> bool
        {
            // If ray is parallel to slab, check if origin is inside slab
            if (std::abs(dA) < 1e-6f)
                return (oA >= minA && oA <= maxA);

            float t1 = (minA - oA) / dA;
            float t2 = (maxA - oA) / dA;
            if (t1 > t2) std::swap(t1, t2);

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            return tmin <= tmax;
        };

    // Check intersection on all three axes
    if (!update(min_.x, max_.x, ray.origin.x, ray.direction.x) ||
        !update(min_.y, max_.y, ray.origin.y, ray.direction.y) ||
        !update(min_.z, max_.z, ray.origin.z, ray.direction.z))
    {
        return hp; // No intersection
    }

    // Choose nearest valid intersection distance
    float t = (tmin > 0.0f) ? tmin : tmax;
    if (t < 0.0f) return hp;

    // Fill HitPoint data
    hp.hit = true;
    hp.distance = t;
    hp.name = name_;
    hp.color = color_;
    hp.direction = ray.direction;
    hp.position = ray.origin + t * ray.direction;

    return hp;
}