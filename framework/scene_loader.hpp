#pragma once
#include <string>
#include "Scene.hpp"

bool load_scene_from_sdf(const std::string& filename, Scene& scene);
inline bool loadSceneFromSDF(const std::string& filename, Scene& scene) {
    return load_scene_from_sdf(filename, scene);
}
