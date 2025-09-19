#include <iostream>
#include <string>
#include "scene_loader.hpp"
#include "renderer.hpp"
#include "ppmwriter.hpp"

int main(int argc, char** argv){
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <scene.sdf> <output.ppm> [width height]\n";
        return 0;
    }
    std::string scene_file = argv[1];
    std::string out_file = argv[2];
    int W = (argc > 3) ? std::stoi(argv[3]) : 800;
    int H = (argc > 4) ? std::stoi(argv[4]) : 600;

    Scene scene;
    if(!load_scene_from_sdf(scene_file, scene)){
        std::cerr << "Failed to load scene\n";
        return 1;
    }

    RenderSettings settings; settings.width = W; settings.height = H;
    Renderer renderer(scene, settings);
    auto pixels = renderer.render();
    if(!ppm::writeP6(out_file, W, H, pixels)){
        std::cerr << "Failed to write " << out_file << "\n";
        return 1;
    }
    std::cout << "Rendered " << out_file << " ("<<W<<"x"<<H<<")\n";
    return 0;
}
