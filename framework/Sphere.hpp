#include "Shape.hpp"
#include <glm/vec3.hpp>

// Sphere class inherits from Shape
class Sphere : public Shape {
private:
    glm::vec3 center; // Center of the sphere
    double radius;    // Radius of the sphere

public:
    // Constructor to initialize center and radius
    Sphere(const glm::vec3& center, double radius);

    // Override area and volume methods
    double area() const override;
    double volume() const override;
};
