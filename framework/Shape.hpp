#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <glm/vec3.hpp>

// Abstract base class for all 3D shapes
class Shape {

protected:
    std::string name_;
    glm::vec3 color_;

public:

    Shape(const std::string& name, const glm::vec3& color)
        : name_(name), color_(color) {}

    virtual ~Shape() {} // Virtual destructor 

    // Pure virtual functions
    virtual double area() const = 0;
    virtual double volume() const = 0;

};

#endif 