#include "Shape.hpp"
#include <glm/vec3.hpp>

// Sphere class inherits from Shape
class Sphere : public Shape {

private:
    glm::vec3 center_;
    double radius_;

public:
    // Constructor to initialize center and radius
    Sphere(const std::string& name, const glm::vec3& color,
        const glm::vec3& center, double radius);

    // Override area and volume methods
    double area() const override;
    double volume() const override;
};
