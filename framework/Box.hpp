#include "Shape.hpp"
#include <glm/vec3.hpp>

// Box class inherits from Shape
class Box : public Shape {
private:
    glm::vec3 min; // Minimum corner
    glm::vec3 max; // Maximum corner

public:
    // Constructor: two corners of the box
    Box(const glm::vec3& min, const glm::vec3& max);

    // Override area and volume methods
    double area() const override;
    double volume() const override;
};

