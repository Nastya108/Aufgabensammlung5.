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