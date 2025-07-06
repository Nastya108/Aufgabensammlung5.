#include "scene_loader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <glm/vec3.hpp>

void loadSceneFromSDF(const std::string& filepath, Scene& scene) {
    std::ifstream in{ filepath };
    if (!in) {
        throw std::runtime_error("Cannot open SDF file: " + filepath);
    }

    // Keep track of materials by name in case shapes refer to them later
    std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream tok(line);
        std::string first;
        if (!(tok >> first) || first == "#") {
            // skip empty lines and comments
            continue;
        }

        if (first == "define") {
            std::string what;
            tok >> what;

            if (what == "material") {
                // parse: define material <name> [Ka] [Kd] [Ks] <m>
                std::string name;
                glm::vec3 ka, kd, ks;
                float m;

                tok >> name
                    >> ka.x >> ka.y >> ka.z
                    >> kd.x >> kd.y >> kd.z
                    >> ks.x >> ks.y >> ks.z
                    >> m;

                auto mat = std::make_shared<Material>(name, ka, kd, ks, m);
                materialMap[name] = mat;
                scene.materials.push_back(mat);
            }
            // other 'define' cases (shape, light, etc.) omitted for this exercise
        }
        // other commands (ambient, camera, render) omitted
    }
}
