#include "Sphere.hpp"
#include <cmath>
#include "Ray.hpp"       
#include "HitPoint.hpp" 
#include <glm/gtx/intersect.hpp>

// Define constant for pi
const double PI = 3.14159265358979323846;

// Constructor initializes sphere with name, color, center and radius
Sphere::Sphere(const std::string& name, const glm::vec3& color,
    const glm::vec3& center, double radius)
    : Shape(name, color), center_(center), radius_(radius) {
}

// Print sphere details to output stream
std::ostream& Sphere::print(std::ostream& os) const {
    Shape::print(os);
    os << ", Center: (" << center_.x << ", " << center_.y << ", " << center_.z << ")"
        << ", Radius: " << radius_;
    return os;
}

// Calculate surface area of the sphere
double Sphere::area() const {
    return 4.0 * PI * radius_ * radius_;
}

// Calculate volume of the sphere
double Sphere::volume() const {
    return (4.0 / 3.0) * PI * radius_ * radius_ * radius_;
}

HitPoint Sphere::intersect(Ray const& ray) const {
    HitPoint hp;
    float t = 0.0f;

    // GLM function, ray direction must be normalized
    bool hit = glm::intersectRaySphere(
        ray.origin,              // Ray origin
        ray.direction,           // Ray direction (normalized)
        center_,                 // Center of the sphere
        static_cast<float>(radius_ * radius_), // Radius squared
        t);                      // Hit distance 

    // If there's a hit and it's in front of the ray origin
    if (hit && t > 0.0f) {
        hp.hit = true;                          // Set hit flag
        hp.distance = t;                        // Store intersection distance
        hp.name = name_;                        // Name of the hit object
        hp.color = color_;                      // Color of the object
        hp.direction = ray.direction;           // Direction of the incoming ray
        hp.position = ray.origin + t * ray.direction; // Hit position
    }
    return hp;
}