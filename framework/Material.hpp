#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <ostream>
#include <glm/vec3.hpp>   // pull in glm::vec3

/**
 * @brief Simple Data‐Transfer Object for material properties.
 */
struct Material {
    std::string name;  ///< material identifier
    glm::vec3   ka;    ///< ambient reflection coefficient
    glm::vec3   kd;    ///< diffuse reflection coefficient
    glm::vec3   ks;    ///< specular reflection coefficient
    float       m;     ///< specular exponent

    Material(std::string const& name,
        glm::vec3 const& ka,
        glm::vec3 const& kd,
        glm::vec3 const& ks,
        float                 m)
        : name(name), ka(ka), kd(kd), ks(ks), m(m)
    {
    }
};

/**
 * @brief Stream‐insertion to print a Material’s fields.
 */
inline std::ostream& operator<<(std::ostream& os, Material const& mat)
{
    os << "Material(name: " << mat.name
        << ", ka: (" << mat.ka.r << ", " << mat.ka.g << ", " << mat.ka.b << ")"
        << ", kd: (" << mat.kd.r << ", " << mat.kd.g << ", " << mat.kd.b << ")"
        << ", ks: (" << mat.ks.r << ", " << mat.ks.g << ", " << mat.ks.b << ")"
        << ", m: " << mat.m << ")";
    return os;
}

#endif // MATERIAL_HPP
