#include "../components/render/src/window.h"

using namespace render;

int main() {
    std::shared_ptr<Window> window = Window::create("Chroma DEMO", -1, -1);
    
    return window->main_loop();
}