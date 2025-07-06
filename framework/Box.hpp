#include "Shape.hpp"
#include <glm/vec3.hpp>
#include "Ray.hpp"
#include "HitPoint.hpp"

#ifndef BOX_HPP
#define BOX_HPP

// Box class inherits from Shape
class Box : public Shape {
private:
    glm::vec3 min_; // Minimum corner
    glm::vec3 max_; // Maximum corner

public:
    // Constructor: two corners of the box, name and color
    Box(const std::string& name, const glm::vec3& color,
        const glm::vec3& min, const glm::vec3& max);

    std::ostream& print(std::ostream& os) const override;

    // Override area and volume methods
    double area() const override;
    double volume() const override;

    //ray‑box intersection
    HitPoint intersect(Ray const& ray) const override;
};

#endif // BOX_HPP

