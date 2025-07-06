#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include "Material.hpp"

/**
 * @brief Simple scene container for shared resources.
 *
 * For Aufgabe 6.5 we only fill the materials vector.
 */
struct Scene {
    std::vector<std::shared_ptr<Material>> materials;
    // later you might add: shapes, lights, camera, etc.
};

#endif // SCENE_HPP
