#include "Shape.hpp"

/**
 * @brief Default print: shows name + either raw color or full material.
 */
std::ostream& Shape::print(std::ostream& os) const
{
    os << "Shape(name: " << name_;
    if (material_) {
        os << ", material: " << *material_;
    }
    else {
        os << ", color: ("
            << color_.r << ", "
            << color_.g << ", "
            << color_.b << ")";
    }
    os << ")";
    return os;
}
