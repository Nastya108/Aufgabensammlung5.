#pragma once
#include <string>
#include "Scene.hpp"

// Поддерживаем две формы имени для совместимости:
// 1) наш load_scene_from_sdf
// 2) ожидаемая вашим проектом loadSceneFromSDF
bool load_scene_from_sdf(const std::string& filename, Scene& scene);
inline bool loadSceneFromSDF(const std::string& filename, Scene& scene) {
    return load_scene_from_sdf(filename, scene);
}

// Формат SDF (поддерживаемый минимум):
// material <name> kd r g b ks r g b ka a shininess s
// sphere <name> cx cy cz radius <material_name>
// box    <name> minx miny minz maxx maxy maxz <material_name>
// light  lx ly lz r g b
// background r g b
// fov deg
// Строки, начинающиеся с '#', — комментарии.
