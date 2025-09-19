#include "scene_loader.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool load_scene_from_sdf(const std::string& filename, Scene& scene) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open scene file: " << filename << std::endl;
        return false;
    }
    std::string line;
    int lineno = 0;

    // Гарантируем дефолтный материал
    if (!scene.findMaterial("default")) {
        auto m = std::make_unique<Material>();
        m->name = "default";
        m->kd = Color3(0.8f);
        m->ks = Color3(0.0f);
        m->ka = 0.0f;
        m->shininess = 32.0f;
        scene.addMaterial(std::move(m));
    }

    while (std::getline(ifs, line)) {
        ++lineno;
        auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        std::istringstream iss(line);
        std::string token;
        if (!(iss >> token)) continue;

        if (token == "material") {
            std::string name;
            if (!(iss >> name)) { std::cerr << "material: missing name at line " << lineno << "\n"; continue; }
            auto m = std::make_unique<Material>();
            m->name = name;
            std::string key;
            while (iss >> key) {
                if (key == "kd") {
                    float r, g, b; if (!(iss >> r >> g >> b)) break;
                    m->kd = Color3(r, g, b);
                }
                else if (key == "ks") {
                    float r, g, b; if (!(iss >> r >> g >> b)) break;
                    m->ks = Color3(r, g, b);
                }
                else if (key == "ka") {
                    float a; if (!(iss >> a)) break;
                    m->ka = a;
                }
                else if (key == "shininess") {
                    float s; if (!(iss >> s)) break;
                    m->shininess = s;
                }
                else {
                    break;
                }
            }
            scene.addMaterial(std::move(m));
        }
        else if (token == "sphere") {
            std::string name, matname;
            float cx, cy, cz, r;
            if (!(iss >> name >> cx >> cy >> cz >> r >> matname)) {
                std::cerr << "sphere: expected: sphere <name> cx cy cz r <material> at line " << lineno << "\n";
                continue;
            }
            const Material* mat = scene.findMaterial(matname);
            if (!mat) { mat = scene.findMaterial("default"); }
            scene.objects.emplace_back(std::make_unique<Sphere>(name, mat, glm::vec3(cx, cy, cz), r));
        }
        else if (token == "box") {
            std::string name, matname;
            float minx, miny, minz, maxx, maxy, maxz;
            if (!(iss >> name >> minx >> miny >> minz >> maxx >> maxy >> maxz >> matname)) {
                std::cerr << "box: expected: box <name> minx miny minz maxx maxy maxz <material> at line " << lineno << "\n";
                continue;
            }
            const Material* mat = scene.findMaterial(matname);
            if (!mat) { mat = scene.findMaterial("default"); }
            scene.objects.emplace_back(std::make_unique<Box>(name, mat, glm::vec3(minx, miny, minz), glm::vec3(maxx, maxy, maxz)));
        }
        else if (token == "light") {
            float x, y, z, r, g, b;
            if (!(iss >> x >> y >> z >> r >> g >> b)) {
                std::cerr << "light: expected: light lx ly lz r g b at line " << lineno << "\n";
                continue;
            }
            PointLight L;
            L.position = glm::vec3(x, y, z);
            L.intensity = Color3(r, g, b);
            scene.lights.push_back(L);
        }
        else if (token == "background") {
            float r, g, b; if (!(iss >> r >> g >> b)) { std::cerr << "background: expected r g b at line " << lineno << "\n"; continue; }
            scene.background = Color3(r, g, b);
        }
        else if (token == "fov") {
            float deg; if (!(iss >> deg)) { std::cerr << "fov: expected degrees at line " << lineno << "\n"; continue; }
            scene.camera.fov_deg = deg;
        }
        else {
            std::cerr << "Unknown token '" << token << "' at line " << lineno << "\n";
        }
    }
    return true;
}
