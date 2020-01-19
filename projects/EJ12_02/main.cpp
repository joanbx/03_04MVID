/*
EJ12.02 - Usar el importador de modelos para crear una escena con algún modelo complejo y pintarlo usando normal mapping con el modelo de iluminación Blinn.
Autor: Joan Baixauli
Ejercicio basado en: AG12
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
#include "engine/geometry/quad.hpp"
#include "engine/model.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(-50.0f, 2.0f, 0.0f);

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

void render(const Model& object, const Shader& s_normal, const Shader& s_phong) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

	lightPos = glm::vec3(std::sin((float)glfwGetTime() / 5.0f), 0.0f, std::abs(std::cos((float)glfwGetTime() / 5.0f)));

	//Modelo phong y normal para comparar

	{
		s_phong.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		s_phong.set("model", model);
		s_phong.set("view", view);
		s_phong.set("proj", proj);

		s_phong.set("light.position", lightPos);
		s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
		s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
		s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);

		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		s_phong.set("normalMat", normalMat);

		s_phong.set("viewPos", camera.getPosition());

		s_phong.set("material.shininess", 32);

		object.render(s_phong);
	}
	
	{
		s_normal.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		s_normal.set("model", model);
		s_normal.set("view", view);
		s_normal.set("proj", proj);
		s_normal.set("light.position", lightPos);
		s_normal.set("light.ambient", 0.1f, 0.1f, 0.1f);
		s_normal.set("light.diffuse", 0.5f, 0.5f, 0.5f);
		s_normal.set("light.specular", 1.0f, 1.0f, 1.0f);


		glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
		s_normal.set("normalMat", normalMat);

		s_normal.set("viewPos", camera.getPosition());

		s_normal.set("material.shininess", 32);

		object.render(s_normal);

	}
	

}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader s_phong("../projects/EJ12_01/phong.vs", "../projects/EJ12_01/blinn.fs");
	const Shader s_normal("../projects/EJ12_01/normal.vs", "../projects/EJ12_01/normal.fs");
	const Model object("../assets/models/Freighter/Freigther_BI_Export.obj");
	


	//const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
	//const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
	//const Texture t_normal("../assets/textures/bricks_normal.png", Texture::Format::RGB);
	//const Quad quad(2.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Input::instance()->setKeyPressedCallback(onKeyPress);
	Input::instance()->setMouseMoveCallback(onMouseMoved);
	Input::instance()->setScrollMoveCallback(onScrollMoved);

	while (window->alive()) {
		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(deltaTime);
		render(object, s_normal, s_phong);
		window->frame();
	}

	return 0;
}