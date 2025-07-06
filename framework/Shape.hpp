#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <ostream>
#include <string>
#include <memory>

#include <glm/vec3.hpp>    // for glm::vec3
#include "Ray.hpp"
#include "HitPoint.hpp"
#include "Material.hpp"    // your Material DTO

/**
 * @brief Abstract base class for all 3D shapes.
 *
 * Supports either a raw color (legacy) or a shared_ptr<Material>.
 */
class Shape {
protected:
    std::string               name_;       ///< name of this shape
    glm::vec3                 color_;      ///< legacy color; used by Box, Sphere, etc.
    std::shared_ptr<Material> material_;   ///< new Material pointer (may be null)

public:
    // Legacy constructor—called by Box, etc., without touching Material.
    Shape(const std::string& name, const glm::vec3& color)
        : name_(name), color_(color), material_(nullptr)
    {
    }

    // New constructor for Material‐based shapes.
    Shape(const std::string& name, std::shared_ptr<Material> mat)
        : name_(name)
        , color_(mat ? mat->kd : glm::vec3{ 1.0f,1.0f,1.0f })  // diffuse as fallback
        , material_(std::move(mat))
    {
    }

    virtual ~Shape() = default;

    /**
     * @brief Print shape name and either color or material.
     */
    virtual std::ostream& print(std::ostream& os) const;

    /// Compute surface area
    virtual double   area()    const = 0;
    /// Compute volume
    virtual double   volume()  const = 0;
    /// Ray–object intersection
    virtual HitPoint intersect(Ray const& ray) const = 0;
};

/**
 * @brief Global stream‐insertion for any Shape.
 */
inline std::ostream& operator<<(std::ostream& os, Shape const& s) {
    return s.print(os);
}

#endif // SHAPE_HPP
