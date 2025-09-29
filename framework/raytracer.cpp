#include <iostream>
#include <string>
#include "scene_loader.hpp"
#include "renderer.hpp"
#include "ppmwriter.hpp"

int main(int argc, char** argv){
    // Check command line arguments
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <scene.sdf> <output.ppm> [width height]\n";
        return 0;
    }
    // Parse command line parameters
    std::string scene_file = argv[1];
    std::string out_file = argv[2];
    int W = (argc > 3) ? std::stoi(argv[3]) : 800;
    int H = (argc > 4) ? std::stoi(argv[4]) : 600;
    int samples = (argc > 5) ? std::stoi(argv[5]) : 1; // Anti-aliasing samples

    // Load scene from SDF file
    Scene scene;
    if(!load_scene_from_sdf(scene_file, scene)){
        std::cerr << "Failed to load scene\n";
        return 1;
    }

    // Configure render settings
    RenderSettings settings;
    settings.width = W;
    settings.height = H;
    settings.samples_per_pixel = samples;

    // Render the scene
    Renderer renderer(scene, settings);
    auto pixels = renderer.render();
    
    // Save output image
    if(!ppm::writeP6(out_file, W, H, pixels)){
        std::cerr << "Failed to write " << out_file << "\n";
        return 1;
    }
    std::cout << "Rendered " << out_file << " (" << W << "x" << H << ") with " << samples << " samples/pixel\n";
    return 0;
}
