#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <ostream>
#include <string>
#include <glm/vec3.hpp>
#include "Ray.hpp"       
#include "HitPoint.hpp"

// Abstract base class for all 3D shapes
class Shape {

protected:
    std::string name_;  // Name of the shape
    glm::vec3 color_;   // Color represented as a 3D vector

public:
    // Constructor to initialize name and color
    Shape(const std::string& name, const glm::vec3& color)
        : name_(name), color_(color) {
    }

    // Prints shape info to the given output stream
    virtual std::ostream& print(std::ostream& os) const;

    // Virtual destructor 
    virtual ~Shape() {} 

    // // Pure virtual methods to compute area and volume
    virtual double area() const = 0;
    virtual double volume() const = 0;

    // Ray/object intersection (pure virtual)
    virtual HitPoint intersect(Ray const& ray) const = 0;

};

// Overload operator<< to print shape using print() method
std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif