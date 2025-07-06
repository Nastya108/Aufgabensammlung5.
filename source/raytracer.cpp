#include <renderer.hpp>
#include <window.hpp>

#include "scene_loader.hpp"   // your SDF loader
#include "Scene.hpp"          // Scene definition

#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <utility>
#include <cmath>

int main(int argc, char* argv[])
{
    // 0) load materials from SDF
    Scene scene;
    try {
        loadSceneFromSDF("scene.sdf", scene);
    }
    catch (std::exception const& e) {
        std::cerr << "Error loading scene: " << e.what() << "\n";
        return 1;
    }

    // just to verify, print how many materials we loaded
    std::cout << "Loaded " << scene.materials.size()
        << " materials from scene.sdf\n";

    // 1) create the checkerboard renderer (three‐arg ctor)
    unsigned const image_width = 800;
    unsigned const image_height = 600;
    std::string const filename = "./checkerboard.ppm";

    Renderer renderer{ image_width,
                       image_height,
                       filename };    // <-- no 'scene' here!

    // 2) render into the PPM
    renderer.render();

    // 3) display in a window
    Window window{ {image_width, image_height} };
    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        window.show(renderer.color_buffer());
    }

    return 0;
}
