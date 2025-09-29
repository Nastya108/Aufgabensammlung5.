#include "scene_loader.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include "Composite.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

static glm::mat4 apply_transform_tokens(std::istringstream& iss) {
    glm::mat4 M(1.0f);
    std::string op;
    while (iss >> op) {
        if (op == "translate") {
            float x, y, z; if (!(iss >> x >> y >> z)) break;
            M = glm::translate(M, glm::vec3(x, y, z));
        }
        else if (op == "scale") {
            float x, y, z; if (!(iss >> x >> y >> z)) break;
            M = glm::scale(M, glm::vec3(x, y, z));
        }
        else if (op == "rotateX") {
            float d; if (!(iss >> d)) break;
            M = glm::rotate(M, d * 3.14159265358979323846f / 180.0f, glm::vec3(1, 0, 0));
        }
        else if (op == "rotateY") {
            float d; if (!(iss >> d)) break;
            M = glm::rotate(M, d * 3.14159265358979323846f / 180.0f, glm::vec3(0, 1, 0));
        }
        else if (op == "rotateZ") {
            float d; if (!(iss >> d)) break;
            M = glm::rotate(M, d * 3.14159265358979323846f / 180.0f, glm::vec3(0, 0, 1));
        }
        else {
            // незнакомый токен — вернёмся к общему парсеру строки
            break;
        }
    }
    return M;
}

bool load_scene_from_sdf(const std::string& filename, Scene& scene) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open scene file: " << filename << std::endl;
        return false;
    }
    std::string line;
    int lineno = 0;

    // дефолтный материал
    if (!scene.findMaterial("default")) {
        auto m = std::make_unique<Material>();
        m->name = "default";
        m->kd = Color3(0.8f);
        m->ks = Color3(0.0f);
        m->ka = 0.0f;
        m->shininess = 32.0f;
        scene.addMaterial(std::move(m));
    }

    std::unordered_map<std::string, std::shared_ptr<Shape>> defined;

    while (std::getline(ifs, line)) {
        ++lineno;
        // обрезаем комментарии
        auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);

        std::istringstream iss(line);
        std::string token;
        if (!(iss >> token)) continue;

        if (token == "material") {
            std::string name;
            if (!(iss >> name)) {
                std::cerr << "material: missing name at line " << lineno << "\n";
                continue;
            }
            auto m = std::make_unique<Material>();
            m->name = name;
            std::string key;
            while (iss >> key) {
                if (key == "kd") { float r, g, b; if (!(iss >> r >> g >> b)) break; m->kd = Color3(r, g, b); }
                else if (key == "ks") { float r, g, b; if (!(iss >> r >> g >> b)) break; m->ks = Color3(r, g, b); }
                else if (key == "ka") { float a; if (!(iss >> a)) break; m->ka = a; }
                else if (key == "shininess") { float s; if (!(iss >> s)) break; m->shininess = s; }
                else if (key == "reflect") { float v; if (!(iss >> v)) break; m->reflect = v; }
                else if (key == "refract") { float v; if (!(iss >> v)) break; m->refract = v; }
                else if (key == "ior") { float v; if (!(iss >> v)) break; m->ior = v; }
                else if (key == "two_sided" || key == "twosided") { m->two_sided = true; }
                else { /* неизвестный ключ → выходим */ break; }
            }
            scene.addMaterial(std::move(m));

        }
        else if (token == "light") {
            float x, y, z, r, g, b;
            if (!(iss >> x >> y >> z >> r >> g >> b)) {
                std::cerr << "light: lx ly lz r g b at line " << lineno << "\n";
                continue;
            }
            scene.lights.push_back({ glm::vec3(x,y,z), Color3(r,g,b) });

        }
        else if (token == "background") {
            float r, g, b;
            if (!(iss >> r >> g >> b)) {
                std::cerr << "background r g b at line " << lineno << "\n";
                continue;
            }
            scene.background = Color3(r, g, b);

        }
        else if (token == "fov") {
            float deg;
            if (!(iss >> deg)) {
                std::cerr << "fov deg at line " << lineno << "\n";
                continue;
            }
            scene.camera.fov_deg = deg;
            scene.camera.eye = glm::vec3(0, 0, 0);
            scene.camera.dir = glm::vec3(0, 0, -1);
            scene.camera.up = glm::vec3(0, 1, 0);

        }
        else if (token == "camera") {
            // camera <name> <fov-x> <ex ey ez> <dx dy dz> <ux uy uz>
            std::string name; float fovx;
            float ex, ey, ez, dx, dy, dz, ux, uy, uz;
            if (!(iss >> name >> fovx >> ex >> ey >> ez >> dx >> dy >> dz >> ux >> uy >> uz)) {
                std::cerr << "camera: camera <name> <fov-x> <ex ey ez> <dx dy dz> <ux uy uz> at line " << lineno << "\n";
                continue;
            }
            scene.camera.fov_deg = fovx; // горизонтальный FOV
            scene.camera.eye = glm::vec3(ex, ey, ez);
            scene.camera.dir = glm::normalize(glm::vec3(dx, dy, dz));
            scene.camera.up = glm::normalize(glm::vec3(ux, uy, uz));

        }
        else if (token == "define") {
            std::string sub;
            if (!(iss >> sub) || sub != "shape") {
                std::cerr << "define: expected 'shape' at line " << lineno << "\n";
                continue;
            }
            std::string kind;
            if (!(iss >> kind)) {
                std::cerr << "define shape: missing kind at " << lineno << "\n";
                continue;
            }

            if (kind == "sphere") {
                std::string name, matname;
                float cx, cy, cz, r;
                if (!(iss >> name >> cx >> cy >> cz >> r >> matname)) {
                    std::cerr << "define shape sphere <name> cx cy cz r <material>\n";
                    continue;
                }
                const Material* mat = scene.findMaterial(matname);
                if (!mat) mat = scene.findMaterial("default");
                auto sp = std::make_shared<Sphere>(name, mat, glm::vec3(cx, cy, cz), r);
                glm::mat4 M = apply_transform_tokens(iss);
                sp->setTransform(M);
                defined[name] = sp;

            }
            else if (kind == "box") {
                std::string name, matname;
                float minx, miny, minz, maxx, maxy, maxz;
                if (!(iss >> name >> minx >> miny >> minz >> maxx >> maxy >> maxz >> matname)) {
                    std::cerr << "define shape box <name> min... max... <material>\n";
                    continue;
                }
                const Material* mat = scene.findMaterial(matname);
                if (!mat) mat = scene.findMaterial("default");
                auto bx = std::make_shared<Box>(name, mat, glm::vec3(minx, miny, minz), glm::vec3(maxx, maxy, maxz));
                glm::mat4 M = apply_transform_tokens(iss);
                bx->setTransform(M);
                defined[name] = bx;

            }
            else if (kind == "composite") {
                std::string name;
                if (!(iss >> name)) {
                    std::cerr << "define shape composite <name> ...children\n";
                    continue;
                }
                auto comp = std::make_shared<Composite>(name);
                std::string child;

                // читаем список детей до первой трансформации
                while (iss >> child) {
                    if (child == "translate" || child == "scale" || child == "rotateX" || child == "rotateY" || child == "rotateZ") {
                        // соберём оставшуюся строку, чтобы применить трансформации
                        std::string rest = child;
                        std::string tmp;
                        std::getline(iss, tmp);
                        rest += tmp;
                        std::istringstream tss(rest);
                        glm::mat4 M = apply_transform_tokens(tss);
                        comp->setTransform(M);
                        break;
                    }
                    auto it = defined.find(child);
                    if (it != defined.end()) {
                        comp->addChild(it->second);
                    }
                    else {
                        std::cerr << "composite '" << name << "' references unknown child '" << child << "' at line " << lineno << "\n";
                    }
                }
                defined[name] = comp;
                if (name == "root" && !scene.root) scene.root = comp;

            }
            else {
                std::cerr << "define shape: unknown kind '" << kind << "' at line " << lineno << "\n";
            }

        }
        else {
            // игнорируем незнакомые токены
        }
    }

    // если root не задан — соберём плоский composite
    if (!scene.root) {
        if (defined.size() == 1) {
            scene.root = defined.begin()->second;
        }
        else if (defined.count("root")) {
            scene.root = defined["root"];
        }
        else {
            auto comp = std::make_shared<Composite>("root");
            for (auto& kv : defined) comp->addChild(kv.second);
            scene.root = comp;
        }
    }
    return true;
}
