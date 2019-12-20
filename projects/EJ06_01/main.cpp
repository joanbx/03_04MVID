/*
EJ06.01 - Sobre la escena del ejercicio 05.04, hacer que la cámara no permita volar y se comporte como una cámara FPS en la cual el personaje siempre estuviera andando sobre el suelo.
Autor: Joan Baixauli
Ejercicio basado en: AG06
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
	Input* input = Input::instance();

	if (input->isKeyPressed(GLFW_KEY_W)) {
		camera.handleKeyboard(Camera::Movement::Forward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_S)) {
		camera.handleKeyboard(Camera::Movement::Backward, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_A)) {
		camera.handleKeyboard(Camera::Movement::Left, dt);
	}
	if (input->isKeyPressed(GLFW_KEY_D)) {
		camera.handleKeyboard(Camera::Movement::Right, dt);
	}

	

}

void onKeyPress(int key, int action) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(true);
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		Window::instance()->setCaptureMode(false);
	}
}

void onMouseMoved(float x, float y) {
	if (firstMouse) {
		firstMouse = false;
		lastX = x;
		lastY = y;
	}

	const float xoffset = x - lastX;
	const float yoffset = lastY - y;
	lastX = x;
	lastY = y;
	
	camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
	camera.handleMouseScroll(y);
}



void render(const Geometry& geom, glm::vec3 cubes[][3], const Shader& shader, Texture* tex) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	for (uint8_t i = 0; i < 10; i++) {

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, cubes[i][0]);

		if (i != 0)
			model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), cubes[i][1]);
		else
			model = glm::rotate(model, glm::radians(0.0f), cubes[i][1]);

		model = glm::scale(model, cubes[i][2]);

		shader.use();

		if (i == 0)
			tex[0].use(shader, "tex", 0);
		else
			tex[1].use(shader, "tex", 0);


		shader.set("model", model);
		shader.set("view", camera.getViewMatrix());
		shader.set("proj", proj);  //TODO const mat4

		geom.render();
	}
}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader shader("../projects/EJ06_01/vertex.vs", "../projects/EJ06_01/fragment.fs");
	const Cube cube(0.5f);
	//const Sphere sphere(1.0f, 50, 50);

	Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Creamos 10 cubos, el Primero será el plano
	glm::vec3 cubePRS[][3] = {
		{ //Cube 01 // Plano
			glm::vec3(-0.0f,-1.0f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(50.f,1.0f,50.0f), //scale
		},
		{ //Cube 02 (Encima de Cubo 01)
			glm::vec3(-0.1f,-0.5f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		},
		{ //Cube 03 (Encima de Cubo 01)
			glm::vec3(1.0f,-0.25f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(2.0f,2.0f,2.0f), //scale
		},
		{ //Cube 04 (Encima de Cubo 01)
			glm::vec3(-0.75f,-0.55f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(0.75f,0.75f,0.75f), //scale
		},
		{ //Cube 05 (Encima de Cubo 01)
			glm::vec3(-1.25f,-0.6f,-3.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(0.5f,0.5f,0.5f), //scale
		},
		{ //Cube 06 (Encima de Cubo 01)
			glm::vec3(0.0f,0.2f,-8.0f), //Position
			glm::vec3(0.0f,1.0f,0.0f), //Rotation
			glm::vec3(3.5f,3.5f,3.5f), //scale
		},
		{ //Cube 07 - Flotando
			glm::vec3(-0.0f,0.9f,-3.0f), //Position
			glm::vec3(0.5f,1.0f,0.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		},
		{ //Cube 08 - Flotando
			glm::vec3(1.0f,1.0f,-3.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(0.25f,0.25f,0.25f), //scale
		},
		{ //Cube 09 - Flotando
			glm::vec3(-2.0f,2.0f,-6.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(0.5f,0.5f,0.5f), //scale
		},
		{ //Cube 10 - Flotando 
			glm::vec3(-4.0f,0.5f,-12.0f), //Position
			glm::vec3(0.5f,1.0f,2.0f), //Rotation
			glm::vec3(1.0f,1.0f,1.0f), //scale
		}
	};
	glEnable(GL_DEPTH_TEST); //IMPORTANTE!

	Texture textures[2]{
		Texture("../assets/textures/grey.png", Texture::Format::RGB),
		Texture("../assets/textures/blue_blocks.jpg", Texture::Format::RGB)
	};

	Input::instance()->setKeyPressedCallback(onKeyPress);
	Input::instance()->setMouseMoveCallback(onMouseMoved);
	Input::instance()->setScrollMoveCallback(onScrollMoved);

	camera.setFPS(true); // Ver Camera

	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(deltaTime);
		render(cube, cubePRS, shader, textures);
		window->frame();
	}

	return 0;
}