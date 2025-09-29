#include "scene_loader.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

bool load_scene_from_sdf(const std::string& filename, Scene& scene) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open scene file: " << filename << std::endl;
        return false;
    }
    std::string line;
    int lineno = 0;

    // Ensure default material exists
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

                else if (key == "reflectivity") {
                    float refl; if (!(iss >> refl)) break;
                    m->reflectivity = refl; // Material parsing for reflectivity
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

        else if (token == "camera") {
            std::string name;
            float fov_x, ex, ey, ez, dx, dy, dz, ux, uy, uz;

            if (!(iss >> name >> fov_x >> ex >> ey >> ez >> dx >> dy >> dz >> ux >> uy >> uz)) {
                std::cerr << "camera: expected: camera <name> <fov-x> <eye_x eye_y eye_z> <dir_x dir_y dir_z> <up_x up_y up_z> at line " << lineno << "\n";
                continue;
            }

            scene.camera.eye = glm::vec3(ex, ey, ez);
            scene.camera.dir = glm::normalize(glm::vec3(dx, dy, dz));
            scene.camera.up = glm::normalize(glm::vec3(ux, uy, uz));
            scene.camera.fov_deg = fov_x;
        }
        else if (token == "transform") {
            std::string objName, type;
            if (!(iss >> objName >> type)) {
                std::cerr << "transform: missing object name or type at line " << lineno << "\n";
                continue;
            }


            // Find object by name
            auto it = std::find_if(scene.objects.begin(), scene.objects.end(),
                [&](const std::unique_ptr<Shape>& o) { return o->name() == objName; });
            if (it == scene.objects.end()) {
                std::cerr << "transform: object '" << objName << "' not found at line " << lineno << "\n";
                continue;
            }

            glm::mat4 t = (*it)->transform();

            // Apply transformation based on type
            if (type == "translate") {
                float x, y, z;
                if (!(iss >> x >> y >> z)) { std::cerr << "translate: missing parameters at line " << lineno << "\n"; continue; }
                t = glm::translate(t, glm::vec3(x, y, z));
            }
            else if (type == "scale") {
                float s;
                if (!(iss >> s)) { std::cerr << "scale: missing parameter at line " << lineno << "\n"; continue; }
                t = glm::scale(t, glm::vec3(s));
            }
            else if (type == "rotate") {
                float angle, x, y, z;
                if (!(iss >> angle >> x >> y >> z)) { std::cerr << "rotate: missing parameters at line " << lineno << "\n"; continue; }
                t = glm::rotate(t, glm::radians(angle), glm::vec3(x, y, z));
            }
            else {
                std::cerr << "transform: unknown type '" << type << "' at line " << lineno << "\n";
                continue;
            }

            (*it)->setTransform(t);
        }
        else {
            std::cerr << "Unknown token '" << token << "' at line " << lineno << "\n";
        }
    }
    return true;
}
