#include "Box.hpp"
#include <cmath>

// Constructor: initialize min and max points
Box::Box(const glm::vec3& min_, const glm::vec3& max_)
    : min(min_), max(max_) {
}

// Compute surface area of the box
double Box::area() const {
    double dx = max.x - min.x;
    double dy = max.y - min.y;
    double dz = max.z - min.z;

    return 2.0 * (dx * dy + dy * dz + dz * dx);
}

// Compute volume of the box
double Box::volume() const {
    double dx = max.x - min.x;
    double dy = max.y - min.y;
    double dz = max.z - min.z;

    return dx * dy * dz;
}
