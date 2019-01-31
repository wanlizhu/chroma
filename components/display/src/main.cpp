#include <display/window.h>
#include <vulkan_instance.h>

using namespace gui;
using namespace numeric;
using namespace render;
using namespace vk;

int main() {
	std::shared_ptr<Window> window = Window::Builder()
			.title("Chroma DEMO")
			.size(int2(1000, 618))
			.create();

    std::shared_ptr<Instance> ins = Instance::create("Chroma", Instance::EXTENSIONS, Instance::LAYERS);
    ins->set_callback();
    auto s = ins->physical_devices(true);

	window->main_loop();
	
	return 0;
}