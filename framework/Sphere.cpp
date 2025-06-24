#include "Sphere.hpp"
#include <cmath>

// Define a constant value for pi
const double PI = 3.14159265358979323846;

// Constructor: initialize sphere with center and radius
Sphere::Sphere(const glm::vec3& center, double radius)
    : center(center), radius(radius) {
}

// Surface area of the sphere
double Sphere::area() const {
    return 4.0 * PI * radius * radius;
}

// Volume of the sphere
double Sphere::volume() const {
    return (4.0 / 3.0) * PI * radius * radius * radius;
}