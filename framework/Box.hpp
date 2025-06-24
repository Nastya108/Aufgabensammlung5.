#include "Shape.hpp"
#include <glm/vec3.hpp>

// Box class inherits from Shape
class Box : public Shape {
private:
    glm::vec3 min; // Minimum corner
    glm::vec3 max; // Maximum corner

public:
    // Constructor: two corners of the box
    Box(const std::string& name, const glm::vec3& color,
        const glm::vec3& min_, const glm::vec3& max_);

    // Override area and volume methods
    double area() const override;
    double volume() const override;
};

