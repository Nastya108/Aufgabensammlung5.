#include "Shape.hpp"
#include <iostream>   // std::cout, std::ostream
#include "Ray.hpp"

// Base print: name + RGB color
std::ostream& Shape::print(std::ostream& os) const
{
    // Output the shape's name
    os << "Shape(name: " << name_
        // Output the color in (R, G, B) format
        << ", color: (" << color_.r << ", "
        << color_.g << ", "
        << color_.b << "))";
    return os;
}

std::ostream& operator<<(std::ostream& os, Shape const& s) {
    s.print(os);
    return os; 
}