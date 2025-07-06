#include "Shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape {
private:
    glm::vec3 center_; // Center position of the sphere
    double radius_;   // Radius of the sphere


public:
    // Constructor to initialize center, radius, name and color
    Sphere(const std::string& name, const glm::vec3& color,
        const glm::vec3& center, double radius);

    //Print method to display sphere details
    std::ostream& print(std::ostream& os) const override;

    // Override area and volume methods
    double area() const override;
    double volume() const override;


    // Checks if the given ray intersects with the sphere and returns hit information
    HitPoint intersect(Ray const& ray) const override;

};
