#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>

// Abstract base class for all 3D shapes
class Shape {
public:
    virtual ~Shape() {} // Virtual destructor 

    // Pure virtual functions
    virtual double area() const = 0;
    virtual double volume() const = 0;

};

#endif 