#include "Shape.hpp"
#include <iostream>   // std::cout, std::ostream

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