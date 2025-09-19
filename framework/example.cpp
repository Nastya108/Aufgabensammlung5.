#include <iostream>
#include "scene_loader.hpp"
#include "renderer.hpp"
#include "ppmwriter.hpp"

int main(){
    Scene scene;
    // build a trivial hardcoded scene
    auto m = std::make_unique<Material>();
    m->name = "red"; m->kd = Color3(1,0,0); m->ks = Color3(0.2f); m->shininess=64.0f;
    scene.addMaterial(std::move(m));
    scene.lights.push_back({{3,3,-2}, {10,10,10}});

    scene.objects.emplace_back(std::make_unique<Sphere>("s1", scene.findMaterial("red"), glm::vec3(0,0,-5), 1.0f));

    RenderSettings settings; settings.width = 320; settings.height = 240;
    Renderer r(scene, settings);
    auto img = r.render();
    ppm::writeP6("example.ppm", settings.width, settings.height, img);
    std::cout << "Wrote example.ppm\n";
    return 0;
}
