#include <iostream>
#include <string>
#include "scene_loader.hpp"
#include "renderer.hpp"
#include "ppmwriter.hpp"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <scene.sdf> <output.ppm> [width height]\n";
        return 0;
    }

    const std::string scene_file = argv[1];
    const std::string out_file = argv[2];
    const int W = (argc > 3) ? std::stoi(argv[3]) : 800;
    const int H = (argc > 4) ? std::stoi(argv[4]) : 600;

    Scene scene;
    // Совместимость: у нас есть и load_scene_from_sdf, и обёртка loadSceneFromSDF(...)
    if (!loadSceneFromSDF(scene_file, scene)) {
        std::cerr << "Failed to load scene from: " << scene_file << "\n";
        return 1;
    }

    // ВАЖНО: используем круглые скобки, а не brace-init
    Renderer renderer(W, H, scene);

    // Запускаем рендер и получаем пиксели (а также renderer.color_buffer заполнится)
    auto pixels = renderer.render();

    if (!ppm::writeP6(out_file, W, H, pixels)) {
        std::cerr << "Failed to write " << out_file << "\n";
        return 1;
    }

    std::cout << "Rendered " << out_file << " (" << W << "x" << H << ")\n";
    return 0;
}
