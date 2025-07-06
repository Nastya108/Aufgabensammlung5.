#ifndef SCENE_LOADER_HPP
#define SCENE_LOADER_HPP

#include <string>
#include "Scene.hpp"

/**
 * @brief Parse an SDF file and populate the Scene.
 *
 * This function reads only "define material" lines,
 * creates shared_ptr<Material> instances, and
 * stores them in scene.materials.
 */
void loadSceneFromSDF(const std::string& filepath, Scene& scene);

#endif // SCENE_LOADER_HPP
