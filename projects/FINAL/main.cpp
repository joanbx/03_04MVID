#include <gameplay\game.hpp>
#include <chrono>


void onKeyPress(int key, int action) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(true);
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(false);
	}

}






int main(int, char* []) {

	srand(time(NULL)); //Init random seed

	Input::instance()->setKeyPressedCallback(onKeyPress);

	Game game(600, 800);

	return 0;
}