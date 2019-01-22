#include <display/window.h>

using namespace gui;
using namespace math;

int main() {
	std::shared_ptr<Window> window = Window::Builder()
			.title("Chroma DEMO")
			.size(int2(1000, 618))
			.create();
	window->main_loop();
	
	return 0;
}